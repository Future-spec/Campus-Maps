/*
 * ============================================================
 *  graph.cpp — Campus Accessibility Route Finder
 * ============================================================
 *  Implementation of all graph operations:
 *    - Adjacency list management (add/remove/block paths)
 *    - BFS for fewest-stop route finding
 *    - DFS with backtracking for finding all paths
 *    - Dijkstra's algorithm for shortest-distance route
 *    - Accessibility filtering and analysis
 * ============================================================
 */

#include "graph.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

// ══════════════════════════════════════════════════════════════
//  PRIVATE HELPER: isAccessible
// ══════════════════════════════════════════════════════════════
//  CORE FILTERING FUNCTION used by BFS, DFS, and Dijkstra.
//  Instead of modifying the graph, we check each edge at
//  traversal time. This is efficient and reversible.
//  Blocked paths are ALWAYS inaccessible regardless of profile.
// ══════════════════════════════════════════════════════════════
bool CampusGraph::isAccessible(const Path& path,
                                const AccessibilityConstraints& constraints) const {
    if (path.isBlocked) return false;
    if (constraints.avoidStairs && path.hasStairs)           return false;
    if (constraints.avoidNarrow && path.isNarrow)            return false;
    if (constraints.avoidLiftRequired && path.liftRequired)  return false;
    return true;
}

// ══════════════════════════════════════════════════════════════
//  PRIVATE HELPER: getEdgeDistance
// ══════════════════════════════════════════════════════════════
int CampusGraph::getEdgeDistance(const string& from, const string& to) const {
    auto it = adjList.find(from);
    if (it != adjList.end()) {
        for (const Path& p : it->second) {
            if (p.to == to) return p.distance;
        }
    }
    return -1;
}


// ╔════════════════════════════════════════════════════════════╗
// ║  1. LOCATION MANAGEMENT                                   ║
// ╚════════════════════════════════════════════════════════════╝

void CampusGraph::addLocation(const string& name) {
    if (locations.count(name)) {
        cout << "  [!] Location \"" << name << "\" already exists.\n";
        return;
    }
    locations.insert(name);
    adjList[name];
    cout << "  [+] Location \"" << name << "\" added.\n";
}

bool CampusGraph::locationExists(const string& name) const {
    return locations.count(name) > 0;
}

const set<string>& CampusGraph::getLocations() const {
    return locations;
}


// ╔════════════════════════════════════════════════════════════╗
// ║  2. PATH (EDGE) MANAGEMENT                                ║
// ╚════════════════════════════════════════════════════════════╝

// ──────────────────────────────────────────────────────────────
//  addPath — Adds a BIDIRECTIONAL edge to the adjacency list.
//  Since this is an undirected graph, we store the edge TWICE:
//    adjList["A"] gets a Path to "B"
//    adjList["B"] gets a Path to "A"
// ──────────────────────────────────────────────────────────────
void CampusGraph::addPath(const string& from, const string& to, int distance,
                           bool hasStairs, bool isNarrow, bool liftRequired,
                           bool silent) {
    if (!locationExists(from)) {
        locations.insert(from);
        adjList[from];
    }
    if (!locationExists(to)) {
        locations.insert(to);
        adjList[to];
    }

    for (const Path& p : adjList[from]) {
        if (p.to == to) {
            cout << "  [!] Path between \"" << from << "\" and \""
                 << to << "\" already exists.\n";
            return;
        }
    }

    Path forwardEdge;
    forwardEdge.from         = from;
    forwardEdge.to           = to;
    forwardEdge.distance     = distance;
    forwardEdge.hasStairs    = hasStairs;
    forwardEdge.isNarrow     = isNarrow;
    forwardEdge.liftRequired = liftRequired;
    forwardEdge.isBlocked    = false;

    Path reverseEdge;
    reverseEdge.from         = to;
    reverseEdge.to           = from;
    reverseEdge.distance     = distance;
    reverseEdge.hasStairs    = hasStairs;
    reverseEdge.isNarrow     = isNarrow;
    reverseEdge.liftRequired = liftRequired;
    reverseEdge.isBlocked    = false;

    adjList[from].push_back(forwardEdge);
    adjList[to].push_back(reverseEdge);

    if (!silent) {
        cout << "  [+] Path added: " << from << " <--(" << distance << "m)--> " << to;
        if (hasStairs)    cout << " [Stairs]";
        if (isNarrow)     cout << " [Narrow]";
        if (liftRequired) cout << " [Lift]";
        cout << "\n";
    }
}

// ──────────────────────────────────────────────────────────────
//  removePath — Removes a bidirectional edge.
//  Must remove from BOTH directions to keep graph consistent.
// ──────────────────────────────────────────────────────────────
bool CampusGraph::removePath(const string& from, const string& to) {
    bool removed = false;

    auto& listFrom = adjList[from];
    for (auto it = listFrom.begin(); it != listFrom.end(); ++it) {
        if (it->to == to) { listFrom.erase(it); removed = true; break; }
    }

    auto& listTo = adjList[to];
    for (auto it = listTo.begin(); it != listTo.end(); ++it) {
        if (it->to == from) { listTo.erase(it); break; }
    }

    if (removed)
        cout << "  [-] Path removed: " << from << " <--> " << to << "\n";
    else
        cout << "  [!] No path found between \"" << from
             << "\" and \"" << to << "\".\n";
    return removed;
}

// ──────────────────────────────────────────────────────────────
//  blockPath / unblockPath — Toggle the isBlocked flag.
//  Blocked edges are skipped by isAccessible() during traversal.
// ──────────────────────────────────────────────────────────────
void CampusGraph::blockPath(const string& from, const string& to) {
    bool found = false;
    for (Path& p : adjList[from]) {
        if (p.to == to) { p.isBlocked = true; found = true; break; }
    }
    for (Path& p : adjList[to]) {
        if (p.to == from) { p.isBlocked = true; break; }
    }
    if (found) cout << "  [X] Path BLOCKED: " << from << " <--> " << to << "\n";
    else cout << "  [!] No path found between \"" << from << "\" and \"" << to << "\".\n";
}

void CampusGraph::unblockPath(const string& from, const string& to) {
    bool found = false;
    for (Path& p : adjList[from]) {
        if (p.to == to) { p.isBlocked = false; found = true; break; }
    }
    for (Path& p : adjList[to]) {
        if (p.to == from) { p.isBlocked = false; break; }
    }
    if (found) cout << "  [O] Path UNBLOCKED: " << from << " <--> " << to << "\n";
    else cout << "  [!] No path found between \"" << from << "\" and \"" << to << "\".\n";
}

// ──────────────────────────────────────────────────────────────
//  markAccessibility — Update obstacle flags on an existing edge.
// ──────────────────────────────────────────────────────────────
void CampusGraph::markAccessibility(const string& from, const string& to,
                                     bool hasStairs, bool isNarrow, bool liftRequired) {
    bool found = false;
    for (Path& p : adjList[from]) {
        if (p.to == to) {
            p.hasStairs    = hasStairs;
            p.isNarrow     = isNarrow;
            p.liftRequired = liftRequired;
            found = true;
            break;
        }
    }
    for (Path& p : adjList[to]) {
        if (p.to == from) {
            p.hasStairs    = hasStairs;
            p.isNarrow     = isNarrow;
            p.liftRequired = liftRequired;
            break;
        }
    }
    if (found) {
        cout << "  [~] Accessibility updated for " << from << " <--> " << to << ": ";
        if (hasStairs)    cout << "[Stairs] ";
        if (isNarrow)     cout << "[Narrow] ";
        if (liftRequired) cout << "[Lift] ";
        if (!hasStairs && !isNarrow && !liftRequired) cout << "[No obstacles]";
        cout << "\n";
    } else {
        cout << "  [!] No path found between \"" << from << "\" and \"" << to << "\".\n";
    }
}


// ╔════════════════════════════════════════════════════════════╗
// ║  3. ROUTE FINDING ALGORITHMS                              ║
// ╚════════════════════════════════════════════════════════════╝


// ══════════════════════════════════════════════════════════════
//  BFS ROUTE — Breadth-First Search
// ══════════════════════════════════════════════════════════════
//
//  HOW BFS WORKS:
//    1. Start at the source node, add it to a QUEUE (FIFO).
//    2. Process nodes LEVEL BY LEVEL:
//       - Dequeue the front node.
//       - Enqueue all unvisited, accessible neighbors.
//    3. The FIRST time we reach the destination, we have found
//       the path with the FEWEST STOPS (minimum hops).
//
//  WHY BFS GUARANTEES FEWEST HOPS:
//    BFS explores all nodes at distance k (hops) before nodes
//    at distance k+1. So the first path found has the minimum
//    number of edges.
//
//  NOTE: BFS does NOT guarantee shortest DISTANCE (weight).
//    For shortest weighted distance, use Dijkstra's algorithm.
//
//  DATA STRUCTURES USED:
//    - queue<string>                  — BFS frontier (FIFO)
//    - unordered_map<string,string>   — parent map for path reconstruction
//    - set<string>                    — visited set to avoid revisiting
//
//  TIME COMPLEXITY:  O(V + E)
//  SPACE COMPLEXITY: O(V)
// ══════════════════════════════════════════════════════════════
pair<vector<string>, int> CampusGraph::bfsRoute(
    const string& start, const string& end,
    const AccessibilityConstraints& constraints) {

    if (!locationExists(start) || !locationExists(end)) {
        cout << "  [!] Error: One or both locations do not exist.\n";
        return {{}, -1};
    }
    if (start == end) return {{start}, 0};

    // BFS data structures
    queue<string> q;                        // The BFS queue (FIFO)
    unordered_map<string, string> parent;   // parent[node] = predecessor
    set<string> visited;                    // Track visited nodes

    // Initialize: enqueue the starting node
    q.push(start);
    visited.insert(start);
    parent[start] = "";

    // BFS main loop
    while (!q.empty()) {
        string current = q.front();   // Dequeue front (FIFO)
        q.pop();

        // Explore all neighbors of current node
        for (const Path& path : adjList[current]) {
            if (visited.count(path.to)) continue;          // Already visited
            if (!isAccessible(path, constraints)) continue; // Not accessible

            visited.insert(path.to);
            parent[path.to] = current;

            // Destination found! Since BFS is level-by-level,
            // this is guaranteed to be the fewest-hop path.
            if (path.to == end) {
                // Reconstruct path by following parent pointers
                vector<string> route;
                string node = end;
                while (node != "") {
                    route.push_back(node);
                    node = parent[node];
                }
                reverse(route.begin(), route.end());

                // Calculate total distance along this path
                int totalDist = 0;
                for (size_t i = 0; i + 1 < route.size(); i++) {
                    totalDist += getEdgeDistance(route[i], route[i + 1]);
                }
                return {route, totalDist};
            }

            q.push(path.to);   // Enqueue for future exploration
        }
    }

    return {{}, -1};   // No accessible path exists
}


// ══════════════════════════════════════════════════════════════
//  DFS EXPLORE — Depth-First Search (All Paths)
// ══════════════════════════════════════════════════════════════
//
//  HOW DFS WITH BACKTRACKING WORKS:
//    1. Start at the source. Go as DEEP as possible along one
//       branch before backtracking.
//    2. Use a visited set to avoid cycles within the CURRENT path.
//    3. When we reach the destination, save the current path.
//    4. BACKTRACK: remove the last node from the path and the
//       visited set, then try the next neighbor.
//    5. This way, we explore ALL possible paths.
//
//  WHY BACKTRACKING IS KEY:
//    In standard DFS, once a node is visited, it stays visited.
//    Here, we UNvisit nodes when backtracking so that the same
//    node can appear in DIFFERENT paths.
//
//  DATA STRUCTURES USED:
//    - Recursion stack (implicit stack via function calls)
//    - set<string>    — visited set (modified during backtracking)
//    - vector<string> — current path being built
//
//  TIME COMPLEXITY: O(V!) worst case (exponential — all permutations)
// ══════════════════════════════════════════════════════════════

// Recursive helper
void CampusGraph::dfsHelper(
    const string& current,
    const string& destination,
    const AccessibilityConstraints& constraints,
    set<string>& visited,
    vector<string>& currentPath,
    int currentDist,
    vector<pair<vector<string>, int>>& allPaths) {

    // Cap DFS so dense campus graphs do not explode (V! paths)
    if (allPaths.size() >= 30) return;

    // BASE CASE: reached the destination
    if (current == destination) {
        allPaths.push_back({currentPath, currentDist});
        return;
    }

    // RECURSIVE CASE: try each neighbor
    for (const Path& path : adjList[current]) {
        if (visited.count(path.to)) continue;
        if (!isAccessible(path, constraints)) continue;

        // CHOOSE: add neighbor to current path
        visited.insert(path.to);
        currentPath.push_back(path.to);

        // EXPLORE: recurse deeper
        dfsHelper(path.to, destination, constraints,
                  visited, currentPath,
                  currentDist + path.distance, allPaths);

        // UN-CHOOSE (BACKTRACK): undo the choice so we can
        // explore other branches and find different paths
        currentPath.pop_back();
        visited.erase(path.to);
    }
}

// Public DFS interface
vector<pair<vector<string>, int>> CampusGraph::dfsExplore(
    const string& start, const string& end,
    const AccessibilityConstraints& constraints) {

    if (!locationExists(start) || !locationExists(end)) {
        cout << "  [!] Error: One or both locations do not exist.\n";
        return {};
    }

    vector<pair<vector<string>, int>> allPaths;
    set<string> visited;
    vector<string> currentPath;

    visited.insert(start);
    currentPath.push_back(start);

    dfsHelper(start, end, constraints, visited, currentPath, 0, allPaths);

    // Sort all found paths by distance (shortest first)
    sort(allPaths.begin(), allPaths.end(),
         [](const pair<vector<string>, int>& a,
            const pair<vector<string>, int>& b) {
             return a.second < b.second;
         });

    return allPaths;
}


// ══════════════════════════════════════════════════════════════
//  DIJKSTRA'S ALGORITHM — Shortest Distance Route
// ══════════════════════════════════════════════════════════════
//
//  HOW DIJKSTRA WORKS:
//    1. Assign distance = 0 to start, INF to all others.
//    2. Use a MIN-HEAP (priority queue) to always process the
//       node with the smallest known distance.
//    3. For each neighbor, check if going through the current
//       node gives a shorter path. If yes, UPDATE (relax).
//    4. Repeat until destination reached or all nodes processed.
//
//  WHY DIJKSTRA GUARANTEES SHORTEST DISTANCE:
//    The greedy choice of always expanding the closest node
//    ensures that when a node is dequeued, its distance is final.
//
//  DIFFERENCE FROM BFS:
//    - BFS treats all edges as weight 1 (counts hops only).
//    - Dijkstra considers actual edge weights (distances).
//    - BFS uses a simple queue; Dijkstra uses a priority queue.
//
//  DATA STRUCTURES USED:
//    - priority_queue (min-heap) — processes closest node first
//    - unordered_map<string, int>    — shortest distance to each node
//    - unordered_map<string, string> — parent map for reconstruction
//
//  TIME COMPLEXITY:  O((V + E) log V) with binary heap
//  SPACE COMPLEXITY: O(V)
// ══════════════════════════════════════════════════════════════
pair<vector<string>, int> CampusGraph::dijkstraRoute(
    const string& start, const string& end,
    const AccessibilityConstraints& constraints) {

    if (!locationExists(start) || !locationExists(end)) {
        cout << "  [!] Error: One or both locations do not exist.\n";
        return {{}, -1};
    }
    if (start == end) return {{start}, 0};

    // Min-heap: (distance, location). greater<> makes it a MIN-heap.
    priority_queue<
        pair<int, string>,
        vector<pair<int, string>>,
        greater<pair<int, string>>
    > pq;

    unordered_map<string, int> dist;       // Shortest distance from start
    unordered_map<string, string> parent;   // For path reconstruction

    // Initialize: all distances = INF, start = 0
    for (const string& loc : locations) {
        dist[loc] = INT_MAX;
    }
    dist[start] = 0;
    parent[start] = "";
    pq.push({0, start});

    // Dijkstra main loop
    while (!pq.empty()) {
        // Extract node with SMALLEST distance (greedy step)
        auto [d, u] = pq.top();
        pq.pop();

        // Skip outdated heap entries (lazy deletion)
        if (d > dist[u]) continue;

        // Early exit if destination reached
        if (u == end) break;

        // RELAXATION: try to improve distances to neighbors
        for (const Path& path : adjList[u]) {
            if (!isAccessible(path, constraints)) continue;

            int newDist = dist[u] + path.distance;

            // If shorter path found, update
            if (newDist < dist[path.to]) {
                dist[path.to] = newDist;
                parent[path.to] = u;
                pq.push({newDist, path.to});
            }
        }
    }

    // Check if destination was reached
    if (dist[end] == INT_MAX) return {{}, -1};

    // Reconstruct path from parent pointers
    vector<string> route;
    string node = end;
    while (node != "") {
        route.push_back(node);
        node = parent[node];
    }
    reverse(route.begin(), route.end());

    return {route, dist[end]};
}


// ╔════════════════════════════════════════════════════════════╗
// ║  4. ANALYSIS & DISPLAY                                    ║
// ╚════════════════════════════════════════════════════════════╝

// ══════════════════════════════════════════════════════════════
//  compareRoutes — Side-by-side comparison of all algorithms
// ══════════════════════════════════════════════════════════════
void CampusGraph::compareRoutes(const string& start, const string& end,
                                 const AccessibilityConstraints& constraints) {

    cout << "\n  ============================================================\n";
    cout << "    ROUTE COMPARISON: " << start << " --> " << end << "\n";
    cout << "  ============================================================\n";

    // BFS Result
    cout << "\n  --- BFS (Fewest Stops) ---\n";
    auto [bfsPath, bfsDist] = bfsRoute(start, end, constraints);
    if (bfsPath.empty()) {
        cout << "    No accessible route found.\n";
    } else {
        cout << "    Route: ";
        for (size_t i = 0; i < bfsPath.size(); i++) {
            cout << bfsPath[i];
            if (i + 1 < bfsPath.size()) cout << " -> ";
        }
        cout << "\n    Distance: " << bfsDist << "m | Stops: "
             << bfsPath.size() - 1 << "\n";
    }

    // Dijkstra Result
    cout << "\n  --- Dijkstra (Shortest Distance) ---\n";
    auto [dijkPath, dijkDist] = dijkstraRoute(start, end, constraints);
    if (dijkPath.empty()) {
        cout << "    No accessible route found.\n";
    } else {
        cout << "    Route: ";
        for (size_t i = 0; i < dijkPath.size(); i++) {
            cout << dijkPath[i];
            if (i + 1 < dijkPath.size()) cout << " -> ";
        }
        cout << "\n    Distance: " << dijkDist << "m | Stops: "
             << dijkPath.size() - 1 << "\n";
    }

    // DFS Result
    cout << "\n  --- DFS (All Paths Found) ---\n";
    auto allPaths = dfsExplore(start, end, constraints);
    if (allPaths.empty()) {
        cout << "    No accessible routes found.\n";
    } else {
        cout << "    Total paths found: " << allPaths.size() << "\n";
        int count = 1;
        for (auto& [path, dist] : allPaths) {
            cout << "    " << count++ << ". ";
            for (size_t i = 0; i < path.size(); i++) {
                cout << path[i];
                if (i + 1 < path.size()) cout << " -> ";
            }
            cout << " (" << dist << "m)\n";
        }
    }

    // Comparison Summary
    cout << "\n  --- Summary ---\n";
    if (!bfsPath.empty() && !dijkPath.empty()) {
        if (bfsDist != dijkDist) {
            cout << "    * BFS found " << bfsPath.size() - 1 << " stops / "
                 << bfsDist << "m\n";
            cout << "    * Dijkstra found " << dijkPath.size() - 1 << " stops / "
                 << dijkDist << "m\n";
            cout << "    => BFS minimizes STOPS, Dijkstra minimizes DISTANCE.\n";
            cout << "    => Dijkstra saves " << bfsDist - dijkDist
                 << "m but uses " << (int)(dijkPath.size() - bfsPath.size())
                 << " more stop(s).\n";
        } else {
            cout << "    * Both BFS and Dijkstra found the same optimal route!\n";
        }
    }
    cout << "  ============================================================\n\n";
}

// ══════════════════════════════════════════════════════════════
//  showInaccessible — Find unreachable locations via BFS
// ══════════════════════════════════════════════════════════════
//  Uses BFS from start node with given constraints.
//  Any location NOT reached by BFS is inaccessible.
//  This is a REACHABILITY problem — classic BFS application.
// ══════════════════════════════════════════════════════════════
vector<string> CampusGraph::showInaccessible(
    const string& start,
    const AccessibilityConstraints& constraints) {

    if (!locationExists(start)) {
        cout << "  [!] Start location \"" << start << "\" does not exist.\n";
        return {};
    }

    // BFS to find all reachable locations
    set<string> reachable;
    queue<string> q;

    q.push(start);
    reachable.insert(start);

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        for (const Path& path : adjList[current]) {
            if (reachable.count(path.to)) continue;
            if (!isAccessible(path, constraints)) continue;
            reachable.insert(path.to);
            q.push(path.to);
        }
    }

    // Locations NOT in reachable set are inaccessible
    vector<string> unreachable;
    for (const string& loc : locations) {
        if (!reachable.count(loc)) {
            unreachable.push_back(loc);
        }
    }
    return unreachable;
}

// ══════════════════════════════════════════════════════════════
//  displayGraph — Print the full adjacency list
// ══════════════════════════════════════════════════════════════
void CampusGraph::displayGraph() const {

    cout << "\n  ============================================================\n";
    cout << "    CAMPUS MAP — Adjacency List Representation\n";
    cout << "  ============================================================\n\n";

    if (locations.empty()) {
        cout << "    (No locations in the graph)\n\n";
        return;
    }

    for (const string& loc : locations) {
        cout << "  [" << loc << "]\n";
        auto it = adjList.find(loc);
        if (it == adjList.end() || it->second.empty()) {
            cout << "      (no connections)\n";
        } else {
            for (const Path& p : it->second) {
                cout << "      --> " << setw(20) << left << p.to
                     << " " << setw(4) << right << p.distance << "m";
                if (p.hasStairs)    cout << " [Stairs]";
                if (p.isNarrow)     cout << " [Narrow]";
                if (p.liftRequired) cout << " [Lift]";
                if (p.isBlocked)    cout << " [BLOCKED]";
                cout << "\n";
            }
        }
        cout << "\n";
    }

    cout << "  Total Locations: " << locations.size() << "\n";
    int edgeCount = 0;
    for (const auto& pair : adjList) {
        edgeCount += pair.second.size();
    }
    cout << "  Total Paths: " << edgeCount / 2 << "\n";
    cout << "  ============================================================\n\n";
}
