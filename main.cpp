// main.cpp Ã¢â‚¬â€ Campus Accessibility Route Finder
//
// Simple 3-floor campus layout for DSA project demonstration.
// Run from VS Code terminal: g++ -std=c++17 main.cpp graph.cpp -o campus.exe
//
// CAMPUS LAYOUT:
//
//  Floor 3:  [Seminar Hall] ---- [Physics Lab] ---- [Principal Office]
//                  |                                        |
//               (Stairs)                                  (Lift)
//                  |                                        |
//  Floor 2:  [Computer Lab] --- [Classroom 201] ------ [Staff Room]
//                  |                  |                      |
//               (Stairs)         (Narrow Path)             (Lift)
//                  |                  |                      |
//  Floor 1:   [Main Gate] ------- [Library] ----------- [Canteen]

#include "graph.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Ã¢â€â‚¬Ã¢â€â‚¬ Build the 3-floor campus Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
void buildCampus(CampusGraph& graph) {
    cout << "\n  Building 3-floor campus...\n\n";

    // Ã¢â€â‚¬Ã¢â€â‚¬ Floor 1 (Ground Floor) Ã¢â€â‚¬Ã¢â€â‚¬
    graph.addPath("Main Gate",  "Library",  30);     // accessible corridor
    graph.addPath("Library",    "Canteen",  25);     // accessible corridor
    graph.addPath("Main Gate",  "Canteen",  50);     // accessible corridor

    // Ã¢â€â‚¬Ã¢â€â‚¬ Floor 2 (First Floor) Ã¢â€â‚¬Ã¢â€â‚¬
    graph.addPath("Computer Lab",   "Classroom 201", 20);  // accessible
    graph.addPath("Classroom 201",  "Staff Room",    15);  // accessible
    graph.addPath("Computer Lab",   "Staff Room",    40);  // accessible

    // Ã¢â€â‚¬Ã¢â€â‚¬ Floor 3 (Second Floor) Ã¢â€â‚¬Ã¢â€â‚¬
    graph.addPath("Seminar Hall",  "Physics Lab",       25);  // accessible
    graph.addPath("Physics Lab",   "Principal Office",  20);  // accessible

    // Ã¢â€â‚¬Ã¢â€â‚¬ Between Floor 1 and Floor 2 Ã¢â€â‚¬Ã¢â€â‚¬
    graph.addPath("Main Gate",     "Computer Lab",   35, true, false);  // STAIRS
    graph.addPath("Library",       "Classroom 201",  30, false, true);  // NARROW
    graph.addPath("Canteen",       "Staff Room",     10, false, false); // LIFT (accessible)

    // Ã¢â€â‚¬Ã¢â€â‚¬ Between Floor 2 and Floor 3 Ã¢â€â‚¬Ã¢â€â‚¬
    graph.addPath("Computer Lab",  "Seminar Hall",      35, true, false);  // STAIRS
    graph.addPath("Staff Room",    "Principal Office",   10, false, false); // LIFT (accessible)

    cout << "  Campus ready: 9 locations, 13 paths across 3 floors.\n";
}

// Ã¢â€â‚¬Ã¢â€â‚¬ Print a route result Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
void printRoute(const string& algo, const pair<vector<string>, int>& result) {
    cout << "\n  [" << algo << "]\n";
    if (result.second == -1 || result.first.empty()) {
        cout << "  No accessible route found.\n";
        return;
    }
    cout << "  Route: ";
    for (size_t i = 0; i < result.first.size(); i++) {
        cout << result.first[i];
        if (i + 1 < result.first.size()) cout << " -> ";
    }
    cout << "\n  Distance: " << result.second << " metres"
         << "  |  Stops: " << result.first.size() - 1 << "\n";
}

// Ã¢â€â‚¬Ã¢â€â‚¬ Choose a location from the list Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
string chooseLocation(const CampusGraph& graph, const string& prompt, bool showMenu = true) {
    const auto& locs = graph.getLocations();
    vector<string> names(locs.begin(), locs.end());

    if (showMenu) { cout << "\n  Locations:\n"; for (size_t i = 0; i < names.size(); i++) cout << "    " << i + 1 << ". " << names[i] << "\n"; }

    int choice = 0;
    while (choice < 1 || choice > (int)names.size()) {
        cout << "  " << prompt;
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); choice = 0; }
    }
    cin.ignore(1000, '\n');
    return names[choice - 1];
}

// Ã¢â€â‚¬Ã¢â€â‚¬ Main menu Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
int main() {
    CampusGraph graph;
    buildCampus(graph);

    bool wheelchairMode = false;

    int choice = -1;
    while (choice != 0) {
        cout << "\n  Ã¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢Â\n";
        cout << "  CAMPUS ACCESSIBILITY ROUTE FINDER\n";
        cout << "  Wheelchair mode: " << (wheelchairMode ? "ON (avoids stairs + narrow)" : "OFF") << "\n";
        cout << "  Ã¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢ÂÃ¢â€¢Â\n";
        cout << "  1. Display Campus Map (Adjacency List)\n";
        cout << "  2. Find Route Ã¢â‚¬â€ BFS (Fewest Stops)\n";
        cout << "  3. Find Route Ã¢â‚¬â€ Dijkstra (Shortest Distance)\n";
        cout << "  4. Explore All Paths Ã¢â‚¬â€ DFS (Backtracking)\n";
        cout << "  5. Compare Routes (BFS vs Dijkstra)\n";
        cout << "  6. Show Unreachable Locations\n";
        cout << "  7. Block a Path\n";
        cout << "  8. Unblock a Path\n";
        cout << "  9. Toggle Wheelchair Mode\n";
        cout << "  0. Exit\n";
        cout << "  Enter choice: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); choice = -1; continue; }
        cin.ignore(1000, '\n');

        bool avoidStairs = wheelchairMode;
        bool avoidNarrow = wheelchairMode;

        switch (choice) {

        // Ã¢â€â‚¬Ã¢â€â‚¬ 1. Display adjacency list Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 1:
            graph.displayGraph();
            break;

        // Ã¢â€â‚¬Ã¢â€â‚¬ 2. BFS Ã¢â‚¬â€ fewest stops Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 2: {
            string start = chooseLocation(graph, "Start: ", true);
            string end   = chooseLocation(graph, "Destination: ", false);
            auto result  = graph.bfsRoute(start, end, avoidStairs, avoidNarrow);
            printRoute("BFS Ã¢â‚¬â€ Fewest Stops", result);
            break;
        }

        // Ã¢â€â‚¬Ã¢â€â‚¬ 3. Dijkstra Ã¢â‚¬â€ shortest distance Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 3: {
            string start = chooseLocation(graph, "Start: ", true);
            string end   = chooseLocation(graph, "Destination: ", false);
            auto result  = graph.dijkstraRoute(start, end, avoidStairs, avoidNarrow);
            printRoute("Dijkstra Ã¢â‚¬â€ Shortest Distance", result);
            break;
        }

        // Ã¢â€â‚¬Ã¢â€â‚¬ 4. DFS Ã¢â‚¬â€ all paths with backtracking Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 4: {
            string start = chooseLocation(graph, "Start: ", true);
            string end   = chooseLocation(graph, "Destination: ", false);
            auto paths   = graph.dfsAllPaths(start, end, avoidStairs, avoidNarrow);

            cout << "\n  [DFS Ã¢â‚¬â€ All Paths]\n";
            if (paths.empty()) {
                cout << "  No accessible paths found.\n";
            } else {
                cout << "  Found " << paths.size() << " path(s):\n\n";
                for (size_t i = 0; i < paths.size(); i++) {
                    cout << "  " << i + 1 << ". ";
                    for (size_t j = 0; j < paths[i].first.size(); j++) {
                        cout << paths[i].first[j];
                        if (j + 1 < paths[i].first.size()) cout << " -> ";
                    }
                    cout << "  (" << paths[i].second << "m)\n";
                }
            }
            break;
        }

        // Ã¢â€â‚¬Ã¢â€â‚¬ 5. Compare BFS vs Dijkstra Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 5: {
            string start = chooseLocation(graph, "Start: ", true);
            string end   = chooseLocation(graph, "Destination: ", false);

            auto bfs = graph.bfsRoute(start, end, avoidStairs, avoidNarrow);
            auto dij = graph.dijkstraRoute(start, end, avoidStairs, avoidNarrow);

            cout << "\n  Ã¢â€â‚¬Ã¢â€â‚¬ ROUTE COMPARISON Ã¢â€â‚¬Ã¢â€â‚¬\n";
            printRoute("BFS Ã¢â‚¬â€ Fewest Stops", bfs);
            printRoute("Dijkstra Ã¢â‚¬â€ Shortest Distance", dij);

            if (bfs.second != -1 && dij.second != -1) {
                if (bfs.first == dij.first) cout << "\n  Result: Both algorithms found the exact same route.\n"; else if (bfs.second == dij.second) cout << "\n  Result: Different routes, but equal distance (" << bfs.second << "m).\n";
                else
                    cout << "\n  Result: BFS uses " << bfs.first.size() - 1
                         << " stops (" << bfs.second << "m), Dijkstra uses "
                         << dij.first.size() - 1 << " stops (" << dij.second
                         << "m).\n  BFS minimises stops, Dijkstra minimises distance.\n";
            }
            break;
        }

        // Ã¢â€â‚¬Ã¢â€â‚¬ 6. Find unreachable locations Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 6: {
            string start = chooseLocation(graph, "Check from: ", true);
            auto unreachable = graph.findUnreachable(start, avoidStairs, avoidNarrow);

            cout << "\n  [Unreachable from " << start << "]\n";
            if (unreachable.empty()) {
                cout << "  All locations are reachable!\n";
            } else {
                cout << "  " << unreachable.size() << " location(s) unreachable:\n";
                for (const string& loc : unreachable)
                    cout << "    - " << loc << "\n";
            }
            break;
        }

        // Ã¢â€â‚¬Ã¢â€â‚¬ 7. Block a path Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 7: {
            string from = chooseLocation(graph, "Block from: ", true);
            string to   = chooseLocation(graph, "Block to: ", false);
            if (graph.blockPath(from, to)) cout << "\n  Path BLOCKED: " << from << " <-> " << to << "\n"; else cout << "\n  Error: No path exists between " << from << " and " << to << ".\n";
            break;
        }

        // Ã¢â€â‚¬Ã¢â€â‚¬ 8. Unblock a path Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 8: {
            string from = chooseLocation(graph, "Unblock from: ", true);
            string to   = chooseLocation(graph, "Unblock to: ", false);
            if (graph.unblockPath(from, to)) cout << "\n  Path UNBLOCKED: " << from << " <-> " << to << "\n"; else cout << "\n  Error: No path exists between " << from << " and " << to << ".\n";
            break;
        }

        // Ã¢â€â‚¬Ã¢â€â‚¬ 9. Toggle wheelchair mode Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬Ã¢â€â‚¬
        case 9:
            wheelchairMode = !wheelchairMode;
            cout << "\n  Wheelchair mode: "
                 << (wheelchairMode ? "ON (avoids stairs + narrow paths)" : "OFF")
                 << "\n";
            break;

        case 0:
            cout << "\n  Goodbye!\n\n";
            break;

        default:
            cout << "\n  Invalid choice. Try again.\n";
        }
    }

    return 0;
}
