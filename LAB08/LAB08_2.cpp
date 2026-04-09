#include <iostream>
#include <vector>

using namespace std;

// Q2: Comparison-Counting Sort
vector<int> comparisonCountingSort(const vector<int>& arr) {
    int n = arr.size();
    vector<int> count(n, 0);
    vector<int> sortedArr(n, 0);

    // Count how many elements are smaller than arr[i]
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] < arr[j]) {
                count[j]++;
            } else {
                count[i]++;
            }
        }
    }

    // Place elements in their correct position based on the count
    for (int i = 0; i < n; i++) {
        sortedArr[count[i]] = arr[i];
    }

    return sortedArr;
}

void printArray(const vector<int>& arr) {
    for (int val : arr) cout << val << " ";
    cout << "\n";
}

int main() {
    vector<int> arr = {62, 31, 84, 96, 19, 47};

    cout << "--- Q2: Comparison-Counting Sort ---\n";
    cout << "Unsorted Array: ";
    printArray(arr);

    vector<int> sortedArr = comparisonCountingSort(arr);

    cout << "Sorted Array:   ";
    printArray(sortedArr);

    return 0;
}
