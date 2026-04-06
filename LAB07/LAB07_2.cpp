#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>

using namespace std;

// Sift-Down operation to maintain Max-Heap property
void siftDown(vector<int>& heapArray, int n, int currentIndex) {
    int largestIndex = currentIndex;
    int leftChild = 2 * currentIndex + 1;
    int rightChild = 2 * currentIndex + 2;

    if (leftChild < n && heapArray[leftChild] > heapArray[largestIndex]) {
        largestIndex = leftChild;
    }
    if (rightChild < n && heapArray[rightChild] > heapArray[largestIndex]) {
        largestIndex = rightChild;
    }

    if (largestIndex != currentIndex) {
        swap(heapArray[currentIndex], heapArray[largestIndex]);
        siftDown(heapArray, n, largestIndex); 
    }
}

// Q3: Bottom-up heap construction
void buildMaxHeap(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDown(arr, n, i);
    }
}

// Helper to calculate tree height
int getTreeHeight(int n) {
    return ceil(log2(n + 1));
}

// Recursive function to draw the tree onto a 2D character canvas
void drawNode(vector<string>& canvas, const vector<int>& arr, int index, int row, int col, int offset) {
    if (index >= arr.size()) return;

    // Convert number to string and place it in the canvas centered at 'col'
    string valStr = to_string(arr[index]);
    int startPos = col - valStr.length() / 2;
    for (size_t i = 0; i < valStr.length(); i++) {
        if (startPos + i < canvas[row].length()) {
            canvas[row][startPos + i] = valStr[i];
        }
    }

    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    // Draw left child and connecting line '/'
    if (leftChild < arr.size()) {
        canvas[row + 1][col - offset / 2] = '/';
        drawNode(canvas, arr, leftChild, row + 2, col - offset, offset / 2);
    }

    // Draw right child and connecting line '\'
    if (rightChild < arr.size()) {
        canvas[row + 1][col + offset / 2] = '\\';
        drawNode(canvas, arr, rightChild, row + 2, col + offset, offset / 2);
    }
}

// Main function to setup the canvas and print the top-down tree
void printTreeTopDown(const vector<int>& arr) {
    if (arr.empty()) return;

    int height = getTreeHeight(arr.size());
    int canvasHeight = height * 2; // Rows for nodes + rows for lines
    int canvasWidth = 80;          // Standard terminal width
    
    // Create empty canvas filled with spaces
    vector<string> canvas(canvasHeight, string(canvasWidth, ' '));

    // Calculate initial horizontal spacing based on tree height
    int initialOffset = pow(2, height - 2) * 4; 
    if(initialOffset < 4) initialOffset = 4; // minimum spacing

    // Start drawing from the root (index 0) at row 0, centered horizontally
    drawNode(canvas, arr, 0, 0, canvasWidth / 2, initialOffset);

    // Print the canvas
    for (const auto& row : canvas) {
        // Only print rows that actually contain characters (trim trailing empty space)
        if (row.find_first_not_of(' ') != string::npos) {
            cout << row << "\n";
        }
    }
}

// Helper to print array linearly
void printArray(const vector<int>& arr) {
    for (int val : arr) cout << val << " ";
    cout << "\n";
}

int main() {
    // Generate random array with 2-digit numbers for cleaner visualization
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(10, 99); 

    vector<int> randomArray(10);
    for(int i = 0; i < 10; i++) randomArray[i] = dis(gen);

    cout << "--- Q3: Random Array to Max-Heap ---\n\n";
    
    cout << "Initial Random Array:\n";
    printArray(randomArray);
    
    cout << "\nInitial Tree Structure (Top-Down):\n\n";
    printTreeTopDown(randomArray);
    
    cout << "\n-------------------------------------------------\n";

    buildMaxHeap(randomArray);

    cout << "\nValid Max-Heap Array:\n";
    printArray(randomArray);

    cout << "\nValid Max-Heap Tree Structure (Top-Down):\n\n";
    printTreeTopDown(randomArray);
    cout << "\n";

    return 0;
}
