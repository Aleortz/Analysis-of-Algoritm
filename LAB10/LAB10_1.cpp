#include <iostream>
#include <vector>

using namespace std;

// Q1: Fibonacci without DP
int fibRecursive(int n) {
    // Base cases
    if (n == 1 || n == 2) {
        return 1;
    }
    // Recursive step
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// Q2: Fiboncci with DP
int fibDP(int n, vector<int>&memo){
	if (memo[n] != -1){
		return memo[n];
	}
	int result;
	//Bases Cases
	if ( n==1 || n==2)
		result =1;
	else{
	// Recursive step
		result = fibDP(n-1, memo) + fibDP(n-2, memo);
	}
	memo[n]= result;
	return result;
}
		

int main() {
    cout << "--- Q1: Fibonacci (without DP)---\n";
    
    int n = 25;
    cout << "The " << n << "th Fibonacci number is: " << fibRecursive(n) << "\n";
    
	cout << "--- Q2: Fibonacci (with DP)---\n";

     
    vector<int> memo(n + 1, -1);
    
    cout << "The " << n << "th Fibonacci number is: " << fibDP(n, memo) << "\n";
	
    return 0;
}
