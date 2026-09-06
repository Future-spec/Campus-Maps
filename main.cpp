#include "graph.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

CampusGraph createCampusGraph() {
    CampusGraph graph({
        "Entrance", "Main Corridor", "Lab", "Library",
        "Lift", "Stairs", "Seminar Hall"
    });

    // 7 vertices, 9 undirected weighted edges.
    graph.addPath(0, 1, 10);
    graph.addPath(1, 6, 50);           // fewer stops, but long distance
    graph.addPath(1, 2, 10);
    graph.addPath(2, 3, 10);
    graph.addPath(3, 6, 10);
    graph.addPath(1, 4, 5);
    graph.addPath(4, 6, 20);
    graph.addPath(1, 5, 5, true);      // stairs
    graph.addPath(5, 6, 10, true);     // stairs
    return graph;
}

int readLocation(const CampusGraph& graph, const string& prompt) {
    int location;
    graph.displayLocations();
    cout << prompt;
    cin >> location;
    while (cin.fail() || location < 0 || location >= graph.locationCount()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a valid location number: ";
        cin >> location;
    }
    return location;
}

void printRoute(const CampusGraph& graph, const vector<int>& route) {
    if (route.empty()) {
        cout << "No accessible route found.\n";
        return;
    }
    for (size_t i = 0; i < route.size(); ++i) {
        cout << graph.getLocationName(route[i]);
        if (i + 1 < route.size()) cout << " -> ";
    }
    cout << "\nStops: " << route.size() - 1
         << " | Distance: " << graph.routeDistance(route) << " m\n";
}

int main() {
    CampusGraph graph = createCampusGraph();
    bool wheelchairMode = false;
    int choice = -1;

    cout << "\n=== Campus Accessibility Route Finder ===\n";
    cout << "DSA Project: Graph, DFS, BFS and Dijkstra\n";

    while (choice != 0) {
        cout << "\nWheelchair mode: " << (wheelchairMode ? "ON (stairs skipped)" : "OFF") << "\n";
        cout << "1. Show adjacency list\n";
        cout << "2. DFS traversal\n";
        cout << "3. BFS route (fewest stops)\n";
        cout << "4. Dijkstra route (shortest distance)\n";
        cout << "5. Toggle wheelchair mode\n";
        cout << "0. Exit\nChoose an option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            graph.displayGraph();
        } else if (choice == 2) {
            int start = readLocation(graph, "Start location number: ");
            vector<int> order = graph.dfsTraversal(start, wheelchairMode);
            cout << "DFS visit order: ";
            for (size_t i = 0; i < order.size(); ++i) {
                cout << graph.getLocationName(order[i]);
                if (i + 1 < order.size()) cout << " -> ";
            }
            cout << "\n";
        } else if (choice == 3 || choice == 4) {
            int start = readLocation(graph, "Start location number: ");
            int end = readLocation(graph, "Destination location number: ");
            if (choice == 3) {
                cout << "\nBFS route (fewest stops):\n";
                printRoute(graph, graph.bfsRoute(start, end, wheelchairMode));
            } else {
                cout << "\nDijkstra route (shortest distance):\n";
                printRoute(graph, graph.dijkstraRoute(start, end, wheelchairMode));
            }
        } else if (choice == 5) {
            wheelchairMode = !wheelchairMode;
        } else if (choice != 0) {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
