// main.cpp - Campus Accessibility Route Finder
// DSA project: BFS, DFS, Dijkstra on a small campus graph.
//
// Compile:  g++ -std=c++17 main.cpp graph.cpp -o campus_route_finder
// Run:      ./campus_route_finder

#include "graph.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Small campus where rooms are vertices and corridors are weighted edges.
// Two corridors use stairs; those are the ones wheelchair mode avoids.
void loadCampus(CampusGraph& graph) {
    // locations
    graph.addLocation("Entrance");
    graph.addLocation("Main Corridor");
    graph.addLocation("Lab");
    graph.addLocation("Library");
    graph.addLocation("Lift");
    graph.addLocation("Stairs");
    graph.addLocation("Seminar Hall");

    // corridors: from, to, distance (metres), hasStairs
    graph.addPath("Entrance",      "Main Corridor", 10);
    graph.addPath("Main Corridor", "Seminar Hall",  50);
    graph.addPath("Main Corridor", "Lab",           10);
    graph.addPath("Lab",           "Library",       10);
    graph.addPath("Library",       "Seminar Hall",  10);
    graph.addPath("Main Corridor", "Lift",           5);
    graph.addPath("Lift",          "Seminar Hall",  20);
    graph.addPath("Main Corridor", "Stairs",         5, true);   // stairs
    graph.addPath("Stairs",        "Seminar Hall",  10, true);   // stairs
}

// print one route result nicely
void printRoute(const string& label, const pair<vector<string>, int>& result) {
    const vector<string>& route = result.first;
    int dist = result.second;

    cout << "\n  " << label << ":\n";
    if (route.empty()) {
        cout << "    No accessible route found.\n";
        return;
    }
    cout << "    Route: ";
    for (size_t i = 0; i < route.size(); i++) {
        cout << route[i];
        if (i + 1 < route.size()) cout << " -> ";
    }
    cout << "\n    Distance: " << dist << "m | Stops: " << route.size() - 1 << "\n";
}

// read a location name from stdin
string readLocation(const string& prompt) {
    cout << prompt;
    string name;
    getline(cin, name);
    return name;
}

int main() {
    CampusGraph graph;
    loadCampus(graph);

    bool wheelchairMode = false;   // when true, corridors with stairs are skipped

    cout << "\n=== Campus Accessibility Route Finder ===\n";
    cout << "DSA Project: BFS, DFS, Dijkstra\n\n";

    // show the map once at the start
    graph.displayGraph();

    int choice = -1;
    while (choice != 0) {
        cout << "\n  --- MENU ---\n";
        cout << "  Wheelchair mode: " << (wheelchairMode ? "ON  (avoids stairs)" : "OFF") << "\n";
        cout << "    1. BFS   - fewest stops\n";
        cout << "    2. DFS   - all paths\n";
        cout << "    3. Dijkstra - shortest distance\n";
        cout << "    4. Compare BFS / DFS / Dijkstra\n";
        cout << "    5. Toggle wheelchair mode\n";
        cout << "    0. Exit\n";
        cout << "  Choice: ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string start = readLocation("  Start location: ");
                string end   = readLocation("  End location:   ");
                printRoute("BFS - Fewest Stops", graph.bfsRoute(start, end, wheelchairMode));
                break;
            }
            case 2: {
                string start = readLocation("  Start location: ");
                string end   = readLocation("  End location:   ");
                auto all = graph.dfsExplore(start, end, wheelchairMode);
                cout << "\n  DFS - All Accessible Paths:\n";
                if (all.empty()) {
                    cout << "    No accessible paths found.\n";
                } else {
                    cout << "    Total paths found: " << all.size() << "\n";
                    for (size_t i = 0; i < all.size(); i++) {
                        const vector<string>& path = all[i].first;
                        cout << "    " << (i + 1) << ". ";
                        for (size_t j = 0; j < path.size(); j++) {
                            cout << path[j];
                            if (j + 1 < path.size()) cout << " -> ";
                        }
                        cout << "  (" << all[i].second << "m)\n";
                    }
                }
                break;
            }
            case 3: {
                string start = readLocation("  Start location: ");
                string end   = readLocation("  End location:   ");
                printRoute("Dijkstra - Shortest Distance",
                           graph.dijkstraRoute(start, end, wheelchairMode));
                break;
            }
            case 4: {
                string start = readLocation("  Start location: ");
                string end   = readLocation("  End location:   ");

                printRoute("BFS - Fewest Stops", graph.bfsRoute(start, end, wheelchairMode));
                printRoute("Dijkstra - Shortest Distance",
                           graph.dijkstraRoute(start, end, wheelchairMode));
                auto all = graph.dfsExplore(start, end, wheelchairMode);
                cout << "\n  DFS - All Accessible Paths:\n";
                if (all.empty()) {
                    cout << "    No accessible paths found.\n";
                } else {
                    cout << "    Total paths found: " << all.size() << "\n";
                    for (size_t i = 0; i < all.size(); i++) {
                        const vector<string>& path = all[i].first;
                        cout << "    " << (i + 1) << ". ";
                        for (size_t j = 0; j < path.size(); j++) {
                            cout << path[j];
                            if (j + 1 < path.size()) cout << " -> ";
                        }
                        cout << "  (" << all[i].second << "m)\n";
                    }
                }
                break;
            }
            case 5:
                wheelchairMode = !wheelchairMode;
                cout << "\n  Wheelchair mode is now " << (wheelchairMode ? "ON (avoids stairs)" : "OFF") << ".\n";
                break;
            case 0:
                cout << "\n  Exiting. Thank you!\n\n";
                break;
            default:
                cout << "\n  [!] Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
