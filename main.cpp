/*
 * ============================================================
 *  main.cpp — Campus Accessibility Route Finder
 * ============================================================
 *  Menu-driven program that demonstrates:
 *    - Graph (Adjacency List) for campus representation
 *    - BFS for fewest-stop route finding
 *    - DFS for exploring all possible routes
 *    - Dijkstra for shortest-distance route finding
 *    - Accessibility filtering at traversal time
 *
 *  Campus map: TCET 2nd Floor (A-Wing / B-Wing / C-Wing)
 *
 *  Compile: g++ -std=c++17 main.cpp graph.cpp -o campus_route_finder
 *  Run:     ./campus_route_finder
 * ============================================================
 */

#include "graph.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ──────────────────────────────────────────────────────────────
//  loadDemoData — TCET 2nd Floor (A / B / C wings)
// ──────────────────────────────────────────────────────────────
//  Modelled from the college AutoCAD 2nd-floor plan:
//    A-Wing  — IT staff, computer labs, handicap restroom, fire lift
//    B-Wing  — Guest room, multipurpose hall, auditorium
//    C-Wing  — 210–214 labs, seminar hall, computer centre
//    Central — junction + main lift + lobby stairs
//
//  Demo talking points for viva:
//    1. BFS vs Dijkstra: 210 EXTC Lab-8 -> 213 Computer Centre
//       BFS uses the corridor (fewer stops). Dijkstra uses the
//       lab-to-lab chain (shorter metres).
//    2. Wheelchair: stair landings are unreachable; Auditorium
//       is still reachable via B-Wing Fire Lift.
//    3. Block B-Wing Fire Lift: Auditorium becomes inaccessible
//       (only remaining link is stairs).
// ──────────────────────────────────────────────────────────────
void loadDemoData(CampusGraph& graph) {
    cout << "\n  Loading TCET 2nd Floor campus map...\n";

    const bool S = true;   // silent while bulk-loading

    // Central circulation
    graph.addPath("Central Junction", "A-Wing Corridor",     40, false, false, false, S);
    graph.addPath("Central Junction", "B-Wing Corridor",     35, false, false, false, S);
    graph.addPath("Central Junction", "C-Wing Corridor",     35, false, false, false, S);
    graph.addPath("Central Junction", "Main Lift",           12, false, false, true,  S);
    graph.addPath("Central Junction", "Lobby Stairs",        12, true,  false, false, S);

    // A-Wing: labs sit on a corridor hub + a short lab chain
    graph.addPath("A-Wing Corridor", "IT Department",              22, false, false, false, S);
    graph.addPath("A-Wing Corridor", "IT Staff Room",              18, false, false, false, S);
    graph.addPath("A-Wing Corridor", "System Lab",                 20, false, false, false, S);
    graph.addPath("A-Wing Corridor", "Database Lab",               24, false, false, false, S);
    graph.addPath("A-Wing Corridor", "OS Lab",                     28, false, false, false, S);
    graph.addPath("A-Wing Corridor", "Cloud Computing Lab",        32, false, false, false, S);
    graph.addPath("A-Wing Corridor", "Project Lab",                36, false, false, false, S);
    graph.addPath("A-Wing Corridor", "Software Lab",               26, false, true,  false, S);
    graph.addPath("A-Wing Corridor", "Ladies & Handicap Restroom", 14, false, false, false, S);
    graph.addPath("A-Wing Corridor", "A-Wing Rest Room",           16, false, true,  false, S);
    graph.addPath("A-Wing Corridor", "A-Wing Stairs",              18, true,  false, false, S);
    graph.addPath("A-Wing Corridor", "A-Wing Fire Lift",           16, false, false, true,  S);
    graph.addPath("System Lab",      "Database Lab",               12, false, false, false, S);
    graph.addPath("Database Lab",    "OS Lab",                     12, false, false, false, S);
    graph.addPath("OS Lab",          "Cloud Computing Lab",        12, false, false, false, S);
    graph.addPath("Cloud Computing Lab", "Project Lab",            12, false, false, false, S);

    // B-Wing: large halls; auditorium reachable by stairs OR fire lift
    graph.addPath("B-Wing Corridor", "Guest Room",          20, false, false, false, S);
    graph.addPath("B-Wing Corridor", "Multipurpose Hall",   22, false, false, false, S);
    graph.addPath("B-Wing Corridor", "B-Wing Rest Room",    14, false, false, false, S);
    graph.addPath("B-Wing Corridor", "B-Wing Stairs",       16, true,  false, false, S);
    graph.addPath("B-Wing Corridor", "B-Wing Fire Lift",    18, false, false, true,  S);
    graph.addPath("B-Wing Corridor", "Auditorium Hall",     28, true,  false, false, S);
    graph.addPath("B-Wing Fire Lift", "Auditorium Hall",    14, false, false, true,  S);
    graph.addPath("Guest Room",      "Multipurpose Hall",   18, false, false, false, S);
    graph.addPath("Multipurpose Hall", "Auditorium Hall",   20, true,  false, false, S);

    // C-Wing: 210-214 chain vs corridor (BFS vs Dijkstra demo)
    graph.addPath("C-Wing Corridor", "210 EXTC Lab-8",        18, false, false, false, S);
    graph.addPath("C-Wing Corridor", "211 EXTC Lab-9",        18, false, false, false, S);
    graph.addPath("C-Wing Corridor", "212 Civil & Comp Lab",  18, false, false, false, S);
    graph.addPath("C-Wing Corridor", "213 Computer Centre",   40, false, false, false, S);
    graph.addPath("C-Wing Corridor", "214 Environmental Lab", 22, false, false, false, S);
    graph.addPath("C-Wing Corridor", "Seminar Hall III",      26, false, false, false, S);
    graph.addPath("C-Wing Corridor", "Gents Room",            20, false, false, false, S);
    graph.addPath("C-Wing Corridor", "C-Wing Stairs",         18, true,  false, false, S);
    graph.addPath("C-Wing Corridor", "C-Wing Fire Lift",      16, false, false, true,  S);
    graph.addPath("210 EXTC Lab-8",       "211 EXTC Lab-9",        15, false, false, false, S);
    graph.addPath("211 EXTC Lab-9",       "212 Civil & Comp Lab",  15, false, false, false, S);
    graph.addPath("212 Civil & Comp Lab", "213 Computer Centre",   15, false, false, false, S);
    graph.addPath("214 Environmental Lab", "Seminar Hall III",     16, false, false, false, S);
    graph.addPath("Seminar Hall III",     "C-Wing Fire Lift",      14, false, false, true,  S);

    cout << "  TCET 2nd Floor loaded: A-Wing, B-Wing, C-Wing + central circulation.\n";
    cout << "  Tip: compare BFS vs Dijkstra from \"210 EXTC Lab-8\" to \"213 Computer Centre\".\n\n";
}

// ──────────────────────────────────────────────────────────────
//  selectProfile — Choose an accessibility profile
// ──────────────────────────────────────────────────────────────
AccessibilityConstraints selectProfile() {
    AccessibilityConstraints constraints;

    cout << "\n  ========================================\n";
    cout << "    SELECT ACCESSIBILITY PROFILE\n";
    cout << "  ========================================\n";
    cout << "    [1] Wheelchair User\n";
    cout << "        (Avoids: stairs, narrow, broken lifts)\n";
    cout << "    [2] Visually Impaired\n";
    cout << "        (Avoids: narrow passages)\n";
    cout << "    [3] No Constraints\n";
    cout << "        (Only blocked paths are skipped)\n";
    cout << "    [4] Custom\n";
    cout << "        (Choose what to avoid)\n";
    cout << "  ========================================\n";
    cout << "  Enter choice: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            constraints.avoidStairs       = true;
            constraints.avoidNarrow       = true;
            constraints.avoidLiftRequired = false;
            cout << "  Profile: Wheelchair User\n";
            cout << "  Avoiding: Stairs and narrow passages (lifts are allowed)\n";
            break;
        case 2:
            constraints.avoidStairs       = false;
            constraints.avoidNarrow       = true;
            constraints.avoidLiftRequired = false;
            cout << "  Profile: Visually Impaired\n";
            cout << "  Avoiding: Narrow passages\n";
            break;
        case 3:
            constraints.avoidStairs       = false;
            constraints.avoidNarrow       = false;
            constraints.avoidLiftRequired = false;
            cout << "  Profile: No Constraints\n";
            break;
        case 4: {
            char ch;
            cout << "  Avoid stairs? (y/n): ";
            cin >> ch;
            constraints.avoidStairs = (ch == 'y' || ch == 'Y');
            cout << "  Avoid narrow passages? (y/n): ";
            cin >> ch;
            constraints.avoidNarrow = (ch == 'y' || ch == 'Y');
            cout << "  Avoid paths requiring lifts? (y/n): ";
            cin >> ch;
            constraints.avoidLiftRequired = (ch == 'y' || ch == 'Y');
            cout << "  Profile: Custom\n";
            break;
        }
        default:
            cout << "  Invalid choice. Using No Constraints.\n";
            break;
    }
    return constraints;
}

// ──────────────────────────────────────────────────────────────
//  printProfileInfo — Display current constraints
// ──────────────────────────────────────────────────────────────
void printProfileInfo(const AccessibilityConstraints& c) {
    cout << "  Current Profile: Avoiding [";
    bool any = false;
    if (c.avoidStairs)       { cout << "Stairs"; any = true; }
    if (c.avoidNarrow)       { if (any) cout << ", "; cout << "Narrow"; any = true; }
    if (c.avoidLiftRequired) { if (any) cout << ", "; cout << "Lifts"; any = true; }
    if (!any) cout << "Nothing - no constraints";
    cout << "]\n";
}

// Helper: read a location name (handles spaces in names)
string readLocation(const string& prompt) {
    cout << prompt;
    string name;
    getline(cin, name);
    return name;
}

// Helper: display a single route result
void displayRoute(const string& algorithm,
                  const pair<vector<string>, int>& result) {
    auto& [path, dist] = result;
    cout << "\n  --- " << algorithm << " ---\n";
    if (path.empty()) {
        cout << "    No accessible route found!\n";
    } else {
        cout << "    Route: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i + 1 < path.size()) cout << " -> ";
        }
        cout << "\n    Total Distance: " << dist << "m | Stops: "
             << path.size() - 1 << "\n";
    }
}


// ══════════════════════════════════════════════════════════════
//  MAIN — Program Entry Point
// ══════════════════════════════════════════════════════════════
int main() {
    CampusGraph graph;
    AccessibilityConstraints currentProfile;

    cout << "\n";
    cout << "  ================================================================\n";
    cout << "  |                                                              |\n";
    cout << "  |        CAMPUS ACCESSIBILITY ROUTE FINDER                     |\n";
    cout << "  |        DSA Project - Graph, BFS, DFS, Dijkstra              |\n";
    cout << "  |                                                              |\n";
    cout << "  ================================================================\n\n";

    loadDemoData(graph);
    currentProfile = selectProfile();

    int choice = -1;

    while (choice != 0) {
        cout << "\n  ================================================================\n";
        cout << "    MAIN MENU\n";
        cout << "  ================================================================\n";
        printProfileInfo(currentProfile);
        cout << "  ----------------------------------------------------------------\n";
        cout << "    1.  Display Campus Map (Adjacency List)\n";
        cout << "    2.  Add Location\n";
        cout << "    3.  Add Path\n";
        cout << "    4.  Remove Path\n";
        cout << "    5.  Block / Unblock a Path\n";
        cout << "    6.  Update Accessibility Tags\n";
        cout << "  ----------------------------------------------------------------\n";
        cout << "    7.  Find Route - BFS (Fewest Stops)\n";
        cout << "    8.  Find Route - DFS (All Paths)\n";
        cout << "    9.  Find Route - Dijkstra (Shortest Distance)\n";
        cout << "   10.  Compare All Routes\n";
        cout << "  ----------------------------------------------------------------\n";
        cout << "   11.  Show Inaccessible Locations\n";
        cout << "   12.  Change Accessibility Profile\n";
        cout << "    0.  Exit\n";
        cout << "  ================================================================\n";
        cout << "  Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

            case 1: {
                graph.displayGraph();
                break;
            }

            case 2: {
                string name = readLocation("  Enter location name: ");
                graph.addLocation(name);
                break;
            }

            case 3: {
                string from = readLocation("  Enter source location: ");
                string to   = readLocation("  Enter destination location: ");
                int dist;
                cout << "  Enter distance (metres): ";
                cin >> dist;
                char ch;
                bool stairs, narrow, lift;
                cout << "  Has stairs? (y/n): ";
                cin >> ch;
                stairs = (ch == 'y' || ch == 'Y');
                cout << "  Has narrow passage? (y/n): ";
                cin >> ch;
                narrow = (ch == 'y' || ch == 'Y');
                cout << "  Requires lift? (y/n): ";
                cin >> ch;
                lift = (ch == 'y' || ch == 'Y');
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                graph.addPath(from, to, dist, stairs, narrow, lift);
                break;
            }

            case 4: {
                string from = readLocation("  Enter source location: ");
                string to   = readLocation("  Enter destination location: ");
                graph.removePath(from, to);
                break;
            }

            case 5: {
                string from = readLocation("  Enter source location: ");
                string to   = readLocation("  Enter destination location: ");
                cout << "  [1] Block  [2] Unblock: ";
                int action;
                cin >> action;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (action == 1) graph.blockPath(from, to);
                else             graph.unblockPath(from, to);
                break;
            }

            case 6: {
                string from = readLocation("  Enter source location: ");
                string to   = readLocation("  Enter destination location: ");
                char ch;
                bool stairs, narrow, lift;
                cout << "  Has stairs? (y/n): ";
                cin >> ch;
                stairs = (ch == 'y' || ch == 'Y');
                cout << "  Has narrow passage? (y/n): ";
                cin >> ch;
                narrow = (ch == 'y' || ch == 'Y');
                cout << "  Requires lift? (y/n): ";
                cin >> ch;
                lift = (ch == 'y' || ch == 'Y');
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                graph.markAccessibility(from, to, stairs, narrow, lift);
                break;
            }

            case 7: {
                string from = readLocation("  Enter start location: ");
                string to   = readLocation("  Enter destination: ");
                auto result = graph.bfsRoute(from, to, currentProfile);
                displayRoute("BFS - Fewest Stops", result);
                break;
            }

            case 8: {
                string from = readLocation("  Enter start location: ");
                string to   = readLocation("  Enter destination: ");
                auto allPaths = graph.dfsExplore(from, to, currentProfile);
                cout << "\n  --- DFS - All Accessible Paths ---\n";
                if (allPaths.empty()) {
                    cout << "    No accessible paths found!\n";
                } else {
                    cout << "    Total paths found: " << allPaths.size() << "\n\n";
                    int count = 1;
                    for (auto& [path, dist] : allPaths) {
                        cout << "    " << count++ << ". ";
                        for (size_t i = 0; i < path.size(); i++) {
                            cout << path[i];
                            if (i + 1 < path.size()) cout << " -> ";
                        }
                        cout << "\n       Distance: " << dist << "m | Stops: "
                             << path.size() - 1 << "\n\n";
                    }
                }
                break;
            }

            case 9: {
                string from = readLocation("  Enter start location: ");
                string to   = readLocation("  Enter destination: ");
                auto result = graph.dijkstraRoute(from, to, currentProfile);
                displayRoute("Dijkstra - Shortest Distance", result);
                break;
            }

            case 10: {
                string from = readLocation("  Enter start location: ");
                string to   = readLocation("  Enter destination: ");
                graph.compareRoutes(from, to, currentProfile);
                break;
            }

            case 11: {
                string from = readLocation("  Enter your current location: ");
                auto unreachable = graph.showInaccessible(from, currentProfile);
                cout << "\n  --- Inaccessible Locations ---\n";
                printProfileInfo(currentProfile);
                if (unreachable.empty()) {
                    cout << "    All locations are reachable from \""
                         << from << "\"!\n";
                } else {
                    cout << "    The following " << unreachable.size()
                         << " location(s) are UNREACHABLE from \""
                         << from << "\":\n";
                    for (const string& loc : unreachable) {
                        cout << "      [X] " << loc << "\n";
                    }
                    cout << "\n    These locations cannot be reached with the current\n";
                    cout << "    accessibility constraints. Consider alternative access.\n";
                }
                break;
            }

            case 12: {
                currentProfile = selectProfile();
                break;
            }

            case 0: {
                cout << "\n  Thank you for using Campus Accessibility Route Finder!\n";
                cout << "  Making campuses accessible, one route at a time.\n\n";
                break;
            }

            default: {
                cout << "  [!] Invalid choice. Please try again.\n";
                break;
            }
        }
    }

    return 0;
}
