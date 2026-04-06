#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

// --- HELPER FUNCTIONS ---
void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double val : row) {
            cout << setw(8) << setprecision(3) << fixed << val << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

vector<vector<double>> createAugmentedMatrix(vector<vector<double>> A, const vector<double>& b) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        A[i].push_back(b[i]);
    }
    return A;
}

// --- CORE ALGORITHMS ---

// Q1: Forward Elimination
vector<vector<double>> forwardElimination(vector<vector<double>> augMatrix) {
    int n = augMatrix.size();
    for (int k = 0; k < n - 1; k++) {
        for (int i = k + 1; i < n; i++) {
            double factor = augMatrix[i][k] / augMatrix[k][k];
            for (int j = k; j <= n; j++) {
                augMatrix[i][j] -= factor * augMatrix[k][j];
            }
        }
    }
    return augMatrix;
}

// Q2: Backward Substitution
vector<double> backwardSubstitution(const vector<vector<double>>& augMatrix) {
    int n = augMatrix.size();
    vector<double> x(n, 0.0);

    for (int i = n - 1; i >= 0; i--) {
        double sum = augMatrix[i][n]; 
        for (int j = i + 1; j < n; j++) {
            sum -= augMatrix[i][j] * x[j];
        }
        x[i] = sum / augMatrix[i][i];
    }
    return x;
}

// Q4: LU Decomposition based on Gaussian Elimination
void luDecomposition(vector<vector<double>> A) {
    int n = A.size();
    vector<vector<double>> L(n, vector<double>(n, 0.0));
    vector<vector<double>> U = A; 

    for (int i = 0; i < n; i++) L[i][i] = 1.0; 

    for (int k = 0; k < n - 1; k++) {
        for (int i = k + 1; i < n; i++) {
            double factor = U[i][k] / U[k][k];
            L[i][k] = factor; 
            for (int j = k; j < n; j++) {
                U[i][j] -= factor * U[k][j]; 
            }
        }
    }

    cout << "Lower Matrix (L):\n";
    printMatrix(L);
    cout << "Upper Matrix (U):\n";
    printMatrix(U);
}

// --- BENCHMARK FUNCTION ---
void runPerformanceTest() {
    cout << "\n==================================================\n";
    cout << "   EMPIRICAL COMPLEXITY TEST (BACKWARD SUBSTITUTION)  \n";
    cout << "==================================================\n";
    cout << "N_Size\tTime_Microseconds\n";
    
    // Test for matrix sizes from 1000 to 8000
    for (int n = 1000; n <= 8000; n += 1000) {
        // Create an N x (N+1) augmented upper triangular matrix
        vector<vector<double>> augMatrix(n, vector<double>(n + 1, 1.0));
        
        // Ensure diagonal has non-zero values to avoid division by zero
        for (int i = 0; i < n; i++) {
            augMatrix[i][i] = 5.0; 
        }

        // Measure time strictly for backward substitution
        auto start = high_resolution_clock::now();
        
        // Volatile to prevent compiler from optimizing away the unused return value
        volatile auto result = backwardSubstitution(augMatrix); 
        
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(end - start);
        cout << n << "\t" << duration.count() << "\n";
    }
}

int main() {
    // --- PART 1: Functionality Check ---
    cout << "==================================================\n";
    cout << "            SYSTEM OF EQUATIONS SOLVER            \n";
    cout << "==================================================\n";
    vector<vector<double>> A = {
        {3, 2, -1},
        {2, -2, 4},
        {-1, 0.5, -1}
    };
    vector<double> b = {1, -2, 0};

    cout << "--- Initial State ---\n";
    cout << "Initial Augmented Matrix [A | b]:\n";
    vector<vector<double>> initialAug = createAugmentedMatrix(A, b);
    printMatrix(initialAug);

    cout << "--- Q1: Forward Elimination ---\n";
    cout << "Simplified Augmented Matrix (Upper Triangular):\n";
    vector<vector<double>> simplifiedAug = forwardElimination(initialAug);
    printMatrix(simplifiedAug);

    cout << "--- Q2: Backward Substitution ---\n";
    vector<double> solution = backwardSubstitution(simplifiedAug);
    cout << "Solution Vector (x):\n";
    for (size_t i = 0; i < solution.size(); i++) {
        cout << "x[" << i << "] = " << solution[i] << "\n";
    }
    cout << "\n";

    cout << "--- Q4: LU Decomposition ---\n";
    luDecomposition(A);

    // --- PART 2: Performance Benchmark ---
    runPerformanceTest();

    return 0;
}
