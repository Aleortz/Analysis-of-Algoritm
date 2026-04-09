#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Q4: Hash Table using Closed Hashing (Linear Probing)
class HashTable {
private:
    vector<int> table;
    vector<bool> isOccupied;
    int capacity;
    int currentSize;

    // Simple modulo hash function
    int hashFunction(int key) {
        return key % capacity;
    }

public:
    HashTable(int size) {
        capacity = size;
        currentSize = 0;
        table.assign(capacity, -1); // -1 represents empty slot
        isOccupied.assign(capacity, false);
    }

    // Insert using Linear Probing for collision resolution
    bool insert(int key) {
        if (currentSize == capacity) {
            cout << "Hash Table is Full! Cannot insert " << key << "\n";
            return false;
        }

        int index = hashFunction(key);
        int originalIndex = index;

        // Linear Probing: find next available slot
        while (isOccupied[index]) {
            index = (index + 1) % capacity;
            if (index == originalIndex) return false; // Safety check
        }

        table[index] = key;
        isOccupied[index] = true;
        currentSize++;
        return true;
    }

    // Search for a key
    int search(int key) {
        int index = hashFunction(key);
        int originalIndex = index;

        while (isOccupied[index]) {
            if (table[index] == key) return index; // Found
            index = (index + 1) % capacity;
            if (index == originalIndex) break; // Traversed whole table
        }
        return -1; // Not found
    }

    void display() {
        cout << "Hash Table Structure:\n";
        for (int i = 0; i < capacity; i++) {
            cout << "Index [" << i << "]: ";
            if (isOccupied[i]) {
                cout << table[i] << "\n";
            } else {
                cout << "~~ Empty ~~\n";
            }
        }
    }
};

int main() {
    cout << "--- Q4: Closed Hashing (Linear Probing) ---\n";
    
    HashTable ht(7); // Create hash table of size 7

    // Insert elements (intentionally causing collisions)
    int keys[] = {15, 22, 29, 8, 36}; // 15%7=1, 22%7=1, 29%7=1
    
    for (int key : keys) {
        cout << "Inserting " << key << "...\n";
        ht.insert(key);
    }
    cout << "\n";

    ht.display();

    cout << "\nSearching for 29... Found at index: " << ht.search(29) << "\n";
    cout << "Searching for 99... Found at index: " << ht.search(99) << "\n";

    return 0;
}
