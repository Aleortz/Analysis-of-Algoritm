#include <iostream>
#include <vector>
#include <queue> // Required library for BFS

using namespace std;

// Class representing a Directed Graph using Adjacency Lists
class Graph {
private:
    int V; // Total number of vertices (nodes)
    vector<vector<int>> adjList; // The Adjacency List

    // Recursive helper function (hidden from the end user)
    void DFS_Recursive(int v, vector<bool>& visited) {
        // 1. Mark the current node as visited and process (print) it
        visited[v] = true;
        cout << v << " ";

        // 2. Traverse all neighbors of this node
        for (int neighbor : adjList[v]) {
            // 3. If the neighbor hasn't been visited, dive into it recursively
            if (!visited[neighbor]) {
                DFS_Recursive(neighbor, visited);
            }
        }
    }

public:
    // Constructor: Initializes the graph with 'V' vertices
    Graph(int vertices) {
        V = vertices;
        adjList.resize(V); // Resize the main vector
    }

    // Function to add a directed edge from 'src' to 'dest'
    void addEdge(int src, int dest) {
        adjList[src].push_back(dest);
        // Note: If the graph were UNDIRECTED, we would also add:
        // adjList[dest].push_back(src);
    }

    // Main DFS function called by the user
    void DFS(int startVertex) {
        // Create the visited array initialized to 'false'
        vector<bool> visited(V, false);
        
        cout << "DFS traversal: ";
        // Call the recursive function
        DFS_Recursive(startVertex, visited);
        cout << endl;
    }

    // BFS implementation
    void BFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q; // Our Queue structure

        // Initial step
        visited[startVertex] = true;
        q.push(startVertex);

        cout << "BFS traversal: ";

        // Main loop
        while (!q.empty()) {
            // Remove the front element of the queue
            int currVertex = q.front();
            cout << currVertex << " ";
            q.pop();

            // Check all its direct neighbors (its "level")
            for (int neighbor : adjList[currVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true; // Mark it immediately
                    q.push(neighbor);         // Push it to the back of the queue
                }
            }
        }
        cout << endl;
    }
};

int main() {
    // Create a test graph with 5 nodes (0 to 4)
    Graph g(5);

    // Add some edges
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);

    // Execute the traversals
    g.DFS(0); // Expected output: 0 1 3 4 2
    g.BFS(0); // Expected output: 0 1 2 3 4

    return 0;
}
