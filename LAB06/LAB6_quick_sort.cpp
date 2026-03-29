#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm> // for std::swap

// The Conquer Step: Partitioning the array
int partition(std::vector<int>& arr, int low, int high) {
    // We choose the last element as the pivot
    int pivot = arr[high]; 
    int i = (low - 1); // Index of the smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    // Swap the pivot to its correct mathematical position
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// The Divide Step: Recursive Quick Sort
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        // pi is partitioning index, arr[pi] is now at right place
        int pi = partition(arr, low, high);

        // Separately sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to generate a random vector of a specific size
std::vector<int> generateRandomList(int size) {
    std::vector<int> arr(size);
    // Setup random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 1000000); // Random numbers between 1 and 1,000,000

    for(int i = 0; i < size; i++) {
        arr[i] = distrib(gen);
    }
    return arr;
}

int main() {
    // Array of different list sizes for the empirical analysis (Task 5)
    std::vector<int> listSizes = {10000, 50000, 100000, 500000, 1000000};

    std::cout << "--- Quick Sort Empirical Analysis ---\n";

    for (int n : listSizes) {
        std::cout << "Generating list of " << n << " random elements...\n";
        std::vector<int> arr = generateRandomList(n);

        // 1. Start the high-resolution timer
        auto start = std::chrono::high_resolution_clock::now();

        // 2. Execute Quick Sort
        quickSort(arr, 0, n - 1);

        // 3. Stop the timer
        auto stop = std::chrono::high_resolution_clock::now();
        
        // 4. Calculate duration in milliseconds
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::cout << "-> Sorting completed in: " << duration.count() << " milliseconds.\n\n";
    }

    return 0;
}
