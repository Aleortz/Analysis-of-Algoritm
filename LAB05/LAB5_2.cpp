#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;
using namespace std::chrono;

// Standard Binary Search implementation
int binarySearch(const vector<int>& arr, int key) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// Interpolation Search implementation for uniformly distributed data
int interpolationSearch(const vector<int>& arr, int key) {
    int low = 0, high = arr.size() - 1;
    while (low <= high && key >= arr[low] && key <= arr[high]) {
        if (low == high) {
            return (arr[low] == key) ? low : -1;
        }
        
        // Estimation formula for the position
        int pos = low + (((double)(high - low) / (arr[high] - arr[low])) * (key - arr[low]));
        
        if (arr[pos] == key) return pos;
        if (arr[pos] < key) low = pos + 1;
        else high = pos - 1;
    }
    return -1;
}

int main() {
    const int N = 10000000; // Ten million elements
    vector<int> numbers(N);
    srand(static_cast<unsigned int>(time(0)));

    cout << "Generating and sorting " << N << " elements... (Please wait)" << endl;
    for (int i = 0; i < N; i++) {
        numbers[i] = rand() % 100000000;
    }
    sort(numbers.begin(), numbers.end());

    // Pick a key that exists in the array for testing
    int key = numbers[N / 2 + 500]; 
    cout << "Searching for key: " << key << endl << endl;

    // Measure Binary Search
    auto startBin = high_resolution_clock::now();
    int resultBin = binarySearch(numbers, key);
    auto stopBin = high_resolution_clock::now();
    auto durationBin = duration_cast<nanoseconds>(stopBin - startBin);

    // Measure Interpolation Search
    auto startInt = high_resolution_clock::now();
    int resultInt = interpolationSearch(numbers, key);
    auto stopInt = high_resolution_clock::now();
    auto durationInt = duration_cast<nanoseconds>(stopInt - startInt);

    cout << "--- PERFORMANCE RESULTS ---" << endl;
    cout << "Binary Search Time:        " << durationBin.count() << " ns" << endl;
    cout << "Interpolation Search Time: " << durationInt.count() << " ns" << endl;

    if (resultBin != -1 && resultInt != -1) {
        cout << "\nStatus: Key found successfully in both algorithms." << endl;
    }

    return 0;
}
