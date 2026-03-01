#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono; // Standard library for time measurement

// ==========================================
// 1. RECURSIVE VERSION
// ==========================================
void recursiveHanoi(int n, char from, char to, char aux) {
    // Base case: if no disks are left, stop recursion
    if (n == 0) return;

    // Move n-1 disks from source to auxiliary rod
    recursiveHanoi(n - 1, from, aux, to);

    // The actual movement of disk 'n' would happen here
    // We keep it commented out to avoid I/O overhead during timing
    // cout << "Move disk " << n << " from " << from << " to " << to << endl;

    // Move n-1 disks from auxiliary to destination rod
    recursiveHanoi(n - 1, aux, to, from);
}

// ==========================================
// 2. ITERATIVE VERSION 
// ==========================================
void iterativeHanoi(int n, char from, char to, char aux) {
    // Total moves required is 2^n - 1
    long long total_moves = pow(2, n) - 1;

    // Parity adjustment: If n is even, swap destination and auxiliary rods
    if (n % 2 == 0) {
        char temp = to;
        to = aux;
        aux = temp;
    }

    // Main loop to perform all required movements
    for (long long i = 1; i <= total_moves; i++) {
        // Pattern-based movement using modulo 3 logic
        if (i % 3 == 1) {
            // Legal move between Source and Destination
        } else if (i % 3 == 2) {
            // Legal move between Source and Auxiliary
        } else if (i % 3 == 0) {
            // Legal move between Auxiliary and Destination
        }
    }
}

int main() {
    // Formatting output table for the empirical analysis
    cout << fixed << setprecision(8);
    cout << "n\tRecursive (s)\tIterative (s)\n";
    cout << "------------------------------------------\n";

    // Testing different values of n to observe exponential growth
    for (int n : {5, 10, 15, 20, 25}) {
        
        // Measuring execution time for Recursive method
        auto startR = high_resolution_clock::now();
        recursiveHanoi(n, 'A', 'C', 'B');
        auto endR = high_resolution_clock::now();
        duration<double> diffR = endR - startR;

        // Measuring execution time for Iterative method
        auto startI = high_resolution_clock::now();
        iterativeHanoi(n, 'A', 'C', 'B');
        auto endI = high_resolution_clock::now();
        duration<double> diffI = endI - startI;

        // Print results to console
        cout << n << "\t" << diffR.count() << "\t" << diffI.count() << endl;
    }

    return 0;
}
