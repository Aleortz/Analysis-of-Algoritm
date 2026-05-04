#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <set>
#include <numeric>

using namespace std;

// =====================================================================
// UTILS: LOGGER & RANDOM
// =====================================================================

class Logger {
public:
    ofstream file;
    Logger(const string& filename) {
        file.open(filename, ios::app);
    }
    ~Logger() {
        if (file.is_open()) file.close();
    }
    void log(const string& msg) {
        cout << msg << endl;
        if (file.is_open()) {
            file << msg << endl;
        }
    }
};

Logger logger("grasp_output.txt");

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// =====================================================================
// METRICS DATA STRUCTURES
// =====================================================================

// Struct to store execution metrics for a single run
struct RunMetrics {
    long long best_value;
    double total_runtime;
    double time_to_best;
    vector<long long> convergence_history;
};

// =====================================================================
// MODULE 1: TRAVELING SALESPERSON PROBLEM (TSP)
// =====================================================================

long long calculate_tour_cost(const vector<int>& tour, const vector<vector<int>>& dist_matrix) {
    long long cost = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        cost += dist_matrix[tour[i]][tour[i+1]];
    }
    cost += dist_matrix[tour.back()][tour[0]];
    return cost;
}

vector<int> grasp_tsp_construction(const vector<vector<int>>& dist_matrix, double alpha) {
    int num_cities = dist_matrix.size();
    vector<int> unvisited;
    for (int i = 1; i < num_cities; ++i) unvisited.push_back(i);
    
    vector<int> tour = {0};
    int step = 0;
    
    while (!unvisited.empty()) {
        step++;
        // Suppressed frequent inner logging to avoid flooding during 30x multi-runs
        // if (num_cities > 1000 && step % 1000 == 0) {
        //     logger.log("    [Construction] Connecting city " + to_string(step) + "/" + to_string(num_cities) + "...");
        // }
        
        int current = tour.back();
        int c_min = 2e9, c_max = -1;
        
        for (int city : unvisited) {
            int cost = dist_matrix[current][city];
            if (cost < c_min) c_min = cost;
            if (cost > c_max) c_max = cost;
        }
        
        double threshold = c_min + alpha * (c_max - c_min);
        vector<int> rcl;
        
        for (int city : unvisited) {
            if (dist_matrix[current][city] <= threshold) {
                rcl.push_back(city);
            }
        }
        
        uniform_int_distribution<int> dist(0, rcl.size() - 1);
        int next_city = rcl[dist(rng)];
        
        tour.push_back(next_city);
        unvisited.erase(remove(unvisited.begin(), unvisited.end(), next_city), unvisited.end());
    }
    return tour;
}

vector<int> grasp_tsp_local_search(vector<int> tour, const vector<vector<int>>& dist_matrix, int max_improvements) {
    bool improved = true;
    vector<int> best_tour = tour;
    long long best_cost = calculate_tour_cost(best_tour, dist_matrix);
    
    int improvements_found = 0;
    
    while (improved && (max_improvements == -1 || improvements_found < max_improvements)) {
        improved = false;
        
        for (size_t i = 1; i < best_tour.size() - 2; ++i) {
            for (size_t j = i + 1; j < best_tour.size(); ++j) {
                if (j - i == 1) continue;
                
                vector<int> new_tour = best_tour;
                reverse(new_tour.begin() + i, new_tour.begin() + j); 
                long long new_cost = calculate_tour_cost(new_tour, dist_matrix);
                
                if (new_cost < best_cost) {
                    best_tour = new_tour;
                    best_cost = new_cost;
                    improved = true;
                    improvements_found++;
                    break;
                }
            }
            if (improved) break;
        }
    }
    return best_tour;
}

// Modified to collect execution metrics
RunMetrics run_grasp_tsp(const vector<vector<int>>& dist_matrix, int iterations, double alpha, int max_improvements = -1) {
    RunMetrics metrics;
    metrics.best_value = 2e18; // Infinity representation
    metrics.total_runtime = 0.0;
    metrics.time_to_best = 0.0;
    
    auto start_time = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        vector<int> initial = grasp_tsp_construction(dist_matrix, alpha);
        vector<int> improved = grasp_tsp_local_search(initial, dist_matrix, max_improvements);
        long long c_final = calculate_tour_cost(improved, dist_matrix);
        
        // Track Convergence & Best Time
        if (c_final < metrics.best_value) {
            metrics.best_value = c_final;
            auto current_time = chrono::high_resolution_clock::now();
            metrics.time_to_best = chrono::duration<double>(current_time - start_time).count();
        }
        metrics.convergence_history.push_back(metrics.best_value); // Best value recorded per iteration
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    metrics.total_runtime = chrono::duration<double>(end_time - start_time).count();
    
    return metrics;
}

vector<vector<int>> load_tsplib_matrix(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) return {};
    
    string line;
    bool reading_nodes = false;
    vector<pair<double, double>> coords;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            reading_nodes = true;
            continue;
        } else if (line.find("EOF") != string::npos) break;
        
        if (reading_nodes) {
            stringstream ss(line);
            int id; double x, y;
            if (ss >> id >> x >> y) coords.push_back({x, y});
        }
    }
    
    int num_cities = coords.size();
    if (num_cities == 0) return {};
    
    vector<vector<int>> dist_matrix(num_cities, vector<int>(num_cities, 0));
    for (int i = 0; i < num_cities; ++i) {
        for (int j = 0; j < num_cities; ++j) {
            if (i != j) {
                double dx = coords[i].first - coords[j].first;
                double dy = coords[i].second - coords[j].second;
                dist_matrix[i][j] = round(sqrt(dx*dx + dy*dy));
            }
        }
    }
    return dist_matrix;
}

// =====================================================================
// MODULE 2: 0/1 KNAPSACK PROBLEM
// =====================================================================

long long calculate_knapsack_value(const vector<int>& solution, const vector<int>& values, const vector<int>& weights, long long capacity) {
    long long total_val = 0, total_weight = 0;
    for (int i : solution) {
        total_val += values[i];
        total_weight += weights[i];
    }
    if (total_weight > capacity) return 0;
    return total_val;
}

vector<int> grasp_knapsack_construction(const vector<int>& values, const vector<int>& weights, long long capacity, double alpha) {
    int n = values.size();
    vector<int> items(n);
    iota(items.begin(), items.end(), 0);
    
    vector<int> solution;
    long long current_weight = 0;
    
    while (!items.empty()) {
        vector<int> valid_items;
        for (int i : items) {
            if (current_weight + weights[i] <= capacity) valid_items.push_back(i);
        }
        if (valid_items.empty()) break;
        
        double r_min = 1e18, r_max = -1.0;
        for (int i : valid_items) {
            double r = (double)values[i] / weights[i];
            if (r < r_min) r_min = r;
            if (r > r_max) r_max = r;
        }
        
        double threshold = r_max - alpha * (r_max - r_min);
        vector<int> rcl;
        for (int i : valid_items) {
            if ((double)values[i] / weights[i] >= threshold) rcl.push_back(i);
        }
        
        uniform_int_distribution<int> dist(0, rcl.size() - 1);
        int chosen = rcl[dist(rng)];
        
        solution.push_back(chosen);
        current_weight += weights[chosen];
        items.erase(remove(items.begin(), items.end(), chosen), items.end());
    }
    return solution;
}

vector<int> grasp_knapsack_local_search(vector<int> solution, const vector<int>& values, const vector<int>& weights, long long capacity) {
    bool improved = true;
    vector<int> best_solution = solution;
    long long best_value = calculate_knapsack_value(best_solution, values, weights, capacity);
    int n = values.size();
    
    while (improved) {
        improved = false;
        set<int> items_in(best_solution.begin(), best_solution.end());
        vector<int> items_out;
        for (int i = 0; i < n; ++i) {
            if (items_in.find(i) == items_in.end()) items_out.push_back(i);
        }
        
        for (int i_in : items_in) {
            for (int i_out : items_out) {
                vector<int> temp_solution;
                for (int item : items_in) {
                    if (item != i_in) temp_solution.push_back(item);
                }
                temp_solution.push_back(i_out);
                
                long long temp_value = calculate_knapsack_value(temp_solution, values, weights, capacity);
                if (temp_value > best_value) {
                    best_value = temp_value;
                    best_solution = temp_solution;
                    improved = true;
                    break;
                }
            }
            if (improved) break;
        }
    }
    return best_solution;
}

// Modified to collect execution metrics
RunMetrics run_grasp_knapsack(const vector<int>& values, const vector<int>& weights, long long capacity, int iterations, double alpha) {
    RunMetrics metrics;
    metrics.best_value = -1; 
    metrics.total_runtime = 0.0;
    metrics.time_to_best = 0.0;
    
    auto start_time = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        vector<int> initial = grasp_knapsack_construction(values, weights, capacity, alpha);
        vector<int> improved = grasp_knapsack_local_search(initial, values, weights, capacity);
        long long v_final = calculate_knapsack_value(improved, values, weights, capacity);
        
        // Track Convergence & Best Time
        if (v_final > metrics.best_value) {
            metrics.best_value = v_final;
            auto current_time = chrono::high_resolution_clock::now();
            metrics.time_to_best = chrono::duration<double>(current_time - start_time).count();
        }
        metrics.convergence_history.push_back(metrics.best_value); // Best value recorded per iteration
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    metrics.total_runtime = chrono::duration<double>(end_time - start_time).count();
    
    return metrics;
}

bool load_knapsack_dataset(const string& folder, const string& prefix, vector<int>& values, vector<int>& weights, long long& capacity) {
    ifstream cap_f(folder + prefix + "_c.txt");
    ifstream wt_f(folder + prefix + "_w.txt");
    ifstream val_f(folder + prefix + "_p.txt");
    
    if (!cap_f.is_open() || !wt_f.is_open() || !val_f.is_open()) return false;
    
    cap_f >> capacity;
    int val, wt;
    while (wt_f >> wt) weights.push_back(wt);
    while (val_f >> val) values.push_back(val);
    
    return true;
}


// =====================================================================
// EXPERIMENTAL INSTRUMENTATION & METRICS EXPORT
// =====================================================================

void initialize_output_files() {
    ofstream f_summary("results_summary.txt", ios::trunc);
    f_summary << "instance_name,problem_size,alpha,best,worst,average,std_dev,avg_runtime,avg_time_to_best,avg_gap\n";
    
    ofstream f_conv("convergence.txt", ios::trunc);
    f_conv << "instance_name,alpha,run_id,iteration,best_value\n";
    
    ofstream f_run("per_run.txt", ios::trunc);
    f_run << "instance_name,alpha,run_id,solution_value,total_runtime,time_to_best,gap\n";
    
    ofstream f_alpha("alpha_analysis.txt", ios::trunc);
    f_alpha << "instance_name,problem_size,alpha,avg_value,avg_runtime,avg_gap\n";
}

// Robust evaluation runner for TSP
void evaluate_tsp_robustness(const string& name, const vector<vector<int>>& dist_matrix, long long optimal, int iterations, int num_runs, int max_imp) {
    int n = dist_matrix.size();
    vector<double> test_alphas = {0.1, 0.3, 0.5, 0.7, 0.9}; // 6. Parameter Sensitivity Check
    
    logger.log("\n>>> EVALUATING TSP INSTANCE: " + name + " (Runs: " + to_string(num_runs) + ", Iterations: " + to_string(iterations) + ")");
    
    ofstream f_summary("results_summary.txt", ios::app);
    ofstream f_conv("convergence.txt", ios::app);
    ofstream f_run("per_run.txt", ios::app);
    ofstream f_alpha("alpha_analysis.txt", ios::app);
    
    for (double alpha : test_alphas) {
        vector<long long> best_vals;
        vector<double> total_times;
        vector<double> best_times;
        vector<double> gaps;
        
        for (int r = 1; r <= num_runs; ++r) {
            RunMetrics metrics = run_grasp_tsp(dist_matrix, iterations, alpha, max_imp);
            
            best_vals.push_back(metrics.best_value);
            total_times.push_back(metrics.total_runtime);
            best_times.push_back(metrics.time_to_best);
            
            // 2. Optimality Gap calculation
            double gap = 0.0;
            if (optimal > 0) gap = ((double)(metrics.best_value - optimal) / optimal) * 100.0;
            gaps.push_back(gap);
            
            // Log per-run and convergence data for base parameter only to avoid massive files
            if (alpha == 0.3) {
                f_run << name << "," << alpha << "," << r << "," << metrics.best_value << "," << metrics.total_runtime << "," << metrics.time_to_best << "," << gap << "\n";
                for (size_t it = 0; it < metrics.convergence_history.size(); ++it) {
                    f_conv << name << "," << alpha << "," << r << "," << (it + 1) << "," << metrics.convergence_history[it] << "\n";
                }
            }
        }
        
        // 4. Robustness Statistics Calculation
        long long best_res = *min_element(best_vals.begin(), best_vals.end());
        long long worst_res = *max_element(best_vals.begin(), best_vals.end());
        double avg_res = accumulate(best_vals.begin(), best_vals.end(), 0.0) / num_runs;
        
        double sq_sum = 0;
        for (auto val : best_vals) sq_sum += (val - avg_res) * (val - avg_res);
        double std_dev = sqrt(sq_sum / num_runs);
        
        double avg_time = accumulate(total_times.begin(), total_times.end(), 0.0) / num_runs;
        double avg_time_best = accumulate(best_times.begin(), best_times.end(), 0.0) / num_runs;
        double avg_gap = accumulate(gaps.begin(), gaps.end(), 0.0) / num_runs;
        
        f_alpha << name << "," << n << "," << alpha << "," << avg_res << "," << avg_time << "," << avg_gap << "\n";
        
        if (alpha == 0.3) {
            f_summary << name << "," << n << "," << alpha << "," << best_res << "," << worst_res << "," << avg_res << "," << std_dev << "," << avg_time << "," << avg_time_best << "," << avg_gap << "\n";
            logger.log("    [Alpha " + to_string(alpha) + "] Avg Gap: " + to_string(avg_gap) + "% | Avg Time: " + to_string(avg_time) + "s");
        }
    }
}

// Robust evaluation runner for Knapsack
void evaluate_kp_robustness(const string& name, const vector<int>& vals, const vector<int>& weights, long long cap, long long optimal, int iterations, int num_runs) {
    int n = vals.size();
    vector<double> test_alphas = {0.2, 0.4, 0.6, 0.8}; // 6. Parameter Sensitivity Check
    
    logger.log("\n>>> EVALUATING KNAPSACK INSTANCE: " + name + " (Runs: " + to_string(num_runs) + ", Iterations: " + to_string(iterations) + ")");
    
    ofstream f_summary("results_summary.txt", ios::app);
    ofstream f_conv("convergence.txt", ios::app);
    ofstream f_run("per_run.txt", ios::app);
    ofstream f_alpha("alpha_analysis.txt", ios::app);
    
    for (double alpha : test_alphas) {
        vector<long long> best_vals;
        vector<double> total_times;
        vector<double> best_times;
        vector<double> gaps;
        
        for (int r = 1; r <= num_runs; ++r) {
            RunMetrics metrics = run_grasp_knapsack(vals, weights, cap, iterations, alpha);
            
            best_vals.push_back(metrics.best_value);
            total_times.push_back(metrics.total_runtime);
            best_times.push_back(metrics.time_to_best);
            
            // 2. Optimality Gap calculation (Maximization)
            double gap = 0.0;
            if (optimal > 0) gap = ((double)(optimal - metrics.best_value) / optimal) * 100.0;
            gaps.push_back(gap);
            
            if (alpha == 0.4) {
                f_run << name << "," << alpha << "," << r << "," << metrics.best_value << "," << metrics.total_runtime << "," << metrics.time_to_best << "," << gap << "\n";
                for (size_t it = 0; it < metrics.convergence_history.size(); ++it) {
                    f_conv << name << "," << alpha << "," << r << "," << (it + 1) << "," << metrics.convergence_history[it] << "\n";
                }
            }
        }
        
        // 4. Robustness Statistics Calculation
        long long best_res = *max_element(best_vals.begin(), best_vals.end());
        long long worst_res = *min_element(best_vals.begin(), best_vals.end());
        double avg_res = accumulate(best_vals.begin(), best_vals.end(), 0.0) / num_runs;
        
        double sq_sum = 0;
        for (auto val : best_vals) sq_sum += (val - avg_res) * (val - avg_res);
        double std_dev = sqrt(sq_sum / num_runs);
        
        double avg_time = accumulate(total_times.begin(), total_times.end(), 0.0) / num_runs;
        double avg_time_best = accumulate(best_times.begin(), best_times.end(), 0.0) / num_runs;
        double avg_gap = accumulate(gaps.begin(), gaps.end(), 0.0) / num_runs;
        
        f_alpha << name << "," << n << "," << alpha << "," << avg_res << "," << avg_time << "," << avg_gap << "\n";
        
        if (alpha == 0.4) {
            f_summary << name << "," << n << "," << alpha << "," << best_res << "," << worst_res << "," << avg_res << "," << std_dev << "," << avg_time << "," << avg_time_best << "," << avg_gap << "\n";
            logger.log("    [Alpha " + to_string(alpha) + "] Avg Gap: " + to_string(avg_gap) + "% | Avg Time: " + to_string(avg_time) + "s");
        }
    }
}


// =====================================================================
// MAIN EXECUTION BLOCK
// =====================================================================
int main() {
    initialize_output_files();
    
    logger.log("============================================================");
    logger.log(" EXPERIMENTAL INSTRUMENTATION: GRASP METRICS TRACKING");
    logger.log("============================================================");
    
    string tsp_folder = "ALL_tsp/";
    string knapsack_folder = "knapsack/";
    
    // ---------------------------------------------------------
    // PART 1: TSP Evaluation
    // ---------------------------------------------------------
    vector<vector<int>> dist_kro = load_tsplib_matrix(tsp_folder + "kroA100.tsp");
    if (!dist_kro.empty()) {
        long long opt_kro = 21282;
        // Instance, matrix, optimal, iterations, total runs, max_imp
        evaluate_tsp_robustness("kroA100", dist_kro, opt_kro, 20, 30, -1);
    }
    
    vector<vector<int>> dist_rl = load_tsplib_matrix(tsp_folder + "rl5934.tsp");
    if (!dist_rl.empty()) {
        long long opt_rl = 17462933; // Passed from prior prompt context
        // Executed for 5 runs instead of 30 to prevent extreme benchmark hang
        evaluate_tsp_robustness("rl5934", dist_rl, opt_rl, 2, 5, 500); 
    }

    // ---------------------------------------------------------
    // PART 2: KNAPSACK Evaluation
    // ---------------------------------------------------------
    vector<int> val_p07, wt_p07; long long cap_p07;
    if (load_knapsack_dataset(knapsack_folder, "p07", val_p07, wt_p07, cap_p07)) {
        long long opt_p07 = 1458;
        // Instance, values, weights, capacity, optimal, iterations, total runs
        evaluate_kp_robustness("P07", val_p07, wt_p07, cap_p07, opt_p07, 50, 30);
    }

    vector<int> val_p08, wt_p08; long long cap_p08;
    if (load_knapsack_dataset(knapsack_folder, "p08", val_p08, wt_p08, cap_p08)) {
        long long opt_p08 = 13549094;
        evaluate_kp_robustness("P08", val_p08, wt_p08, cap_p08, opt_p08, 100, 30);
    }

    logger.log("\n============================================================");
    logger.log(" ALL EXPERIMENTS COMPLETE. METRICS WRITTEN TO .TXT FILES.");
    logger.log("============================================================");
    
    return 0;
}
