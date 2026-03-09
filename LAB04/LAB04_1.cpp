#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// Main Algorithm Function
int knapsackExhaustive(int n, int W_max, const vector<int>& weights, const vector<int>& values) {
    int best_value = 0;
    
    // Use 'unsigned long long' to support n >= 32
    unsigned long long total_combinations = 1ULL << n; // This calculates 2^n quickly

    // Iterate through every possible subset
    for (unsigned long long i = 0; i < total_combinations; i++) {
        int current_weight = 0;
        int current_value = 0;

        for (int j = 0; j < n; j++) {
            // Bitwise AND operation to check if the 'j'-th bit is set
            if (i & (1ULL << j)) {
                current_weight += weights[j];
                current_value += values[j];
            }
        }

        // Check if the subset is valid and if it's the best one so far
        if (current_weight <= W_max && current_value > best_value) {
            best_value = current_value;
        }
    }

    return best_value;
}

int main() {
    int n = 31; // Problem size (Start testing with 20, 25, 30...)
    int W_max = 50 * n; // Scaled capacity
    
    // 1. Random data generation according to requirements
    vector<int> weights(n);
    vector<int> values(n);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_weight(10, 100);
    uniform_int_distribution<> dist_value(10, 500);

    for (int i = 0; i < n; i++) {
        weights[i] = dist_weight(gen);
        values[i] = dist_value(gen);
    }

    cout << "Solving Knapsack for n = " << n << "..." << endl;

    // 2. Time measurement
    auto start = high_resolution_clock::now();
    
    int max_val = knapsackExhaustive(n, W_max, weights, values);
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // 3. Results
    cout << "Maximum Value Found: " << max_val << endl;
    cout << "Execution Time: " << duration.count() << " ms" << endl;

    return 0;
}
