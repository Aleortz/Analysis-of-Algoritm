#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

// Implementation of the Insertion Sort Algorithm for characters
void insertionSort(vector<char>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        char key = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void printArray(const vector<char>& arr) {
    for (char c : arr) {
        cout << c << " ";
    }
    cout << endl;
}

int main() {
    // Seed for random number generation
    srand(static_cast<unsigned int>(time(0)));
    
    vector<char> data;
    const int arraySize = 20;

    // Generate 20 random alphabetic characters (a-z)
    for(int i = 0; i < arraySize; i++) {
        data.push_back('a' + rand() % 26);
    }

    cout << "--- EXERCISE 1: INSERTION SORT (ALPHABETIC) ---" << endl;
    cout << "Original array: ";
    printArray(data);

    insertionSort(data);

    cout << "Sorted array:   ";
    printArray(data);

    return 0;
}


