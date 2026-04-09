#include <iostream>
using namespace std;

void swapArithmetic(int& u, int& v) {
    u = u + v;
    v = u - v;
    u = u - v;
}


int main() {
    int u = 15;
    int v = 42;

    cout << "--- Q1: Swap Variables without Extra Storage ---\n";
    cout << "Initial values: u = " << u << ", v = " << v << "\n";

    swapArithmetic(u, v);
    cout << "After  Swap: u = " << u << ", v = " << v << "\n";


    return 0;
}
 
