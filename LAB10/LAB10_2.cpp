#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Q3: 0-1 Knapsack Problem using Bottom-Up Dynamic Programming
int knapsack(int W, const vector<int>& weights, const vector<int>& values, int n) {
    // Create a 2D DP table initialized to 0
    // dp[i][w] represents the maximum value that can be achieved with 'i' items and capacity 'w'
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    // Build the dp table in bottom-up manner
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= W; w++) {
            if (weights[i - 1] <= w) {
                // Item can fit: Take the maximum of including it OR excluding it
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - weights[i - 1]], dp[i - 1][w]);
            } else {
                // Item cannot fit: Must exclude it
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // The bottom-right cell contains the maximum value possible with all items and full capacity
    return dp[n][W];
}

int main() {
    cout << "--- Q3: 0-1 Knapsack Problem (Dynamic Programming) ---\n";
    
    // Example test case
    vector<int> values = {60, 100, 120};  // Values of the items
    vector<int> weights = {10, 20, 30};   // Weights of the items
    int W = 50;                           // Maximum capacity of the knapsack
    int n = values.size();                // Number of items

    cout << "Items configuration (Value, Weight):\n";
    for(int i=0; i<n; i++){
        cout << "Item " << i+1 << ": V=" << values[i] << ", W=" << weights[i] << "\n";
    }
    cout << "Knapsack Capacity: " << W << "\n\n";

    int maxValue = knapsack(W, weights, values, n);
    cout << "Maximum value achieved: " << maxValue << "\n";

    return 0;
}
