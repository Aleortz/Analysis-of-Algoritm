#include <iostream>
#include <vector>
#include <string>

// We use a struct to prove stability. 
// If we have two items with value '2', we can track which one was first.
struct Item {
    int value;
    std::string id;
};

// The Conquer Step: Merging two sorted halves
void merge(std::vector<Item>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Item> L(n1);
    std::vector<Item> R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        // THIS is the line that guarantees stability. 
        // The "<=" ensures that if values are equal, the Left item is placed first.
        if (L[i].value <= R[j].value) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

// The Divide Step: Recursive Merge Sort
void mergeSort(std::vector<Item>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void printArray(const std::vector<Item>& arr) {
    for (const auto& item : arr) {
        std::cout << "[" << item.value << " (" << item.id << ")] ";
    }
    std::cout << "\n";
}

int main() {
    // Array with duplicate values to test stability
    std::vector<Item> arr = {
        {5, "First 5"}, {2, "First 2"}, {9, "Only 9"}, 
        {2, "Second 2"}, {5, "Second 5"}
    };

    std::cout << "Original array:\n";
    printArray(arr);

    mergeSort(arr, 0, arr.size() - 1);

    std::cout << "\nSorted array (Notice the relative order of the '2's and '5's is perfectly maintained):\n";
    printArray(arr);

    return 0;
}
