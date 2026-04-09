#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Q3: Distribution-Counting Sort
vector<int> distributionCountingSort(const vector<int>& arr) {
    if (arr.empty()) return {};

    int n = arr.size();
    
    // Find the maximum value to know the range (k)
    int maxVal = *max_element(arr.begin(), arr.end());
    int minVal = *min_element(arr.begin(), arr.end()); // Handling 0 based arrays simply
    
    // Create frequency array
    vector<int> frequencies(maxVal + 1, 0);
    for (int i = 0; i < n; i++) {
        frequencies[arr[i]]++;
    }

    // Transform to cumulative distribution
    for (int i = 1; i <= maxVal; i++) {
        frequencies[i] += frequencies[i - 1];
    }

    // Place elements into sorted array (backwards to maintain stability)
    vector<int> sortedArr(n, 0);
    for (int i = n - 1; i >= 0; i--) {
        int val = arr[i];
        sortedArr[frequencies[val] - 1] = val;
        frequencies[val]--;
    }

    return sortedArr;
}

void printArray(const vector<int>& arr) {
    for (int val : arr) cout << val << " ";
    cout << "\n";
}

int main() {
    vector<int> arr = {13, 11, 12, 13, 12, 11, 15, 12};

    cout << "--- Q3: Distribution-Counting Sort ---\n";
    cout << "Unsorted Array: ";
    printArray(arr);

    vector<int> sortedArr = distributionCountingSort(arr);

    cout << "Sorted Array:   ";
    printArray(sortedArr);

    return 0;
}
