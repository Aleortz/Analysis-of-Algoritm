#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>

using namespace std;
using namespace std::chrono; // Precision timing

/**
 * Exercise 2: Linear Congruential Generator (LCG)
 * Formula: X_{i+1} = (a * X_i + c) % m
 */
void runSpecificLCG(long long target) {
    // Parameters according to Lab 03 specifications
    unsigned long long m = 4294967296; // 2^32
    unsigned long long a = 1664525;    // Multiplier
    unsigned long long c = 1013904223; // Increment
    unsigned long long x = 42;         // Seed (Initial Value)

    // Start timing the calculation
    auto start = high_resolution_clock::now();

    for (long long i = 1; i <= target; i++) {
        x = (a * x + c) % m; // Calculate next pseudorandom number
    }

    // Stop timing
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    // Display only the final result for this 'n'
    cout << left << setw(12) << target 
         << setw(15) << x 
         << fixed << setprecision(10) << diff.count() << " s" << endl;
}

int main() {
    // Specific iterations requested for the analysis
    long long tests[] = {100, 1000, 10000, 100000, 1000000};

    cout << "------------------------------------------------------------" << endl;
    cout << "  Empirical Analysis: Linear Congruential Generator (LCG)   " << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << left << setw(12) << "n (Iter)" << setw(15) << "Result (X_n)" << "Time (s)" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (long long n : tests) {
        runSpecificLCG(n);
    }

    cout << "------------------------------------------------------------" << endl;
    return 0;
}
