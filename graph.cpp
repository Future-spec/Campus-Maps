// graph.cpp — BFS, DFS and Dijkstra implementations
//
// This file contains three graph traversal algorithms.
// Each one finds a route differently:
//   BFS      → fewest stops (edges)
//   DFS      → every possible route (backtracking)
//   Dijkstra → shortest total distance (metres)

#include "graph.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

// ════════════════════════════════════════════════════════════
//  HELPER FUNCTIONS
// ════════════════════════════════════════════════════════════

// Check if a path can be used based on accessibility constraints.
// This is called by BFS, DFS, and Dijkstra before traversing an edge.
bool CampusGraph::canUse(const Edge& edge, bool avoidStairs, bool avoidNarrow) const {
    if (edge.isBlocked) return false;               // blocked path
    if (avoidStairs && edge.hasStairs) return false; // wheelchair: skip stairs
    if (avoidNarrow && edge.isNarrow) return false;  // wheelchair: skip narrow
    return true;
}

// Get the distance between two directly connected locations.
int CampusGraph::getDistance(const string& from, const string& to) const {
    auto it = adjList.find(from);
    if (it == adjList.end()) return -1;
    for (const Edge& e : it->second) {
        if (e.to == to) return e.distance;
    }
    return -1;
}

// ════════════════════════════════════════════════════════════
//  ADD / REMOVE / DISPLAY
// ════════════════════════════════════════════════════════════

void CampusGraph::addLocation(const string& name) {
    if (locations.count(name) > 0) return;
    locations.insert(name);
    adjList[name]; // create empty edge list
}

// Add a bidirectional path (undirected graph).
// Each edge is stored twice: once in each direction.
void CampusGraph::addPath(const string& from, const string& to,
                          int distance, bool hasStairs, bool isNarrow) {
    addLocation(from);
    addLocation(to);
    adjList[from].push_back({to, distance, hasStairs, isNarrow, false});
    adjList[to].push_back({from, distance, hasStairs, isNarrow, false});
}

// Block a path (mark it as temporarily unavailable).
void CampusGraph::blockPath(const string& from, const string& to) {
    for (Edge& e : adjList[from]) {
        if (e.to == to) { e.isBlocked = true; break; }
    }
    for (Edge& e : adjList[to]) {
        if (e.to == from) { e.isBlocked = true; break; }
    }
}

// Unblock a previously blocked path.
void CampusGraph::unblockPath(const string& from, const string& to) {
    for (Edge& e : adjList[from]) {
        if (e.to == to) { e.isBlocked = false; break; }
    }
    for (Edge& e : adjList[to]) {
        if (e.to == from) { e.isBlocked = false; break; }
    }
}

// Display the entire adjacency list.
void CampusGraph::displayGraph() const {
    cout << "\n  ═══ CAMPUS MAP (Adjacency List) ═══\n\n";
    for (const string& loc : locations) {
        cout << "  [" << loc << "]\n";
        auto it = adjList.find(loc);
        if (it == adjList.end() || it->second.empty()) {
            cout << "      (no connections)\n";
            continue;
        }
        for (const Edge& e : it->second) {
            cout << "      --> " << left << setw(22) << e.to
                 << right << setw(4) << e.distance << "m";
            if (e.hasStairs) cout << "  [Stairs]";
            if (e.isNarrow)  cout << "  [Narrow]";
            if (e.isBlocked) cout << "  [BLOCKED]";
            cout << "\n";
        }
    }
    cout << "\n  Total locations: " << locations.size() << "\n";
}

// ════════════════════════════════════════════════════════════
//  BFS — Breadth-First Search (Fewest Stops)
// ════════════════════════════════════════════════════════════
//
//  HOW IT WORKS:
//    1. Start from the source, push it into a QUEUE.
//    2. Visit all neighbours at distance 1, then distance 2, etc.
//    3. The FIRST time we reach the destination = fewest edges.
//
//  DATA STRUCTURES:
//    - queue<string>          → nodes to visit next (FIFO)
//    - set<string>            → already visited nodes
//    - map<string, string>    → parent of each node (to reconstruct path)
//
//  TIME COMPLEXITY: O(V + E)
//  SPACE COMPLEXITY: O(V)

pair<vector<string>, int> CampusGraph::bfsRoute(
    const string& start, const string& end,
    bool avoidStairs, bool avoidNarrow)
{
    if (!locations.count(start) || !locations.count(end))
        return {{}, -1};
    if (start == end) return {{start}, 0};

    queue<string> q;               // BFS queue
    set<string> visited;           // track visited nodes
    map<string, string> parent;    // parent[x] = who discovered x

    q.push(start);
    visited.insert(start);
    parent[start] = "";

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        // Check all neighbours of current node
        for (const Edge& edge : adjList[current]) {
            if (visited.count(edge.to) > 0) continue;  // skip visited
            if (!canUse(edge, avoidStairs, avoidNarrow)) continue; // skip inaccessible

            visited.insert(edge.to);
            parent[edge.to] = current;

            // Found destination! Reconstruct path by following parents.
            if (edge.to == end) {
                vector<string> route;
                string node = end;
                while (node != "") {
                    route.push_back(node);
                    node = parent[node];
                }
                reverse(route.begin(), route.end());

                // Calculate total distance
                int total = 0;
                for (size_t i = 0; i + 1 < route.size(); i++)
                    total += getDistance(route[i], route[i + 1]);

                return {route, total};
            }

            q.push(edge.to);
        }
    }

    return {{}, -1}; // no route found
}

// ════════════════════════════════════════════════════════════
//  DFS — Depth-First Search with Backtracking (All Paths)
// ════════════════════════════════════════════════════════════
//
//  HOW IT WORKS:
//    1. Go as DEEP as possible along one branch.
//    2. When stuck, BACKTRACK and try another branch.
//    3. After visiting a node, UN-VISIT it so other paths can use it.
//    4. This finds EVERY possible route from start to end.
//
//  DATA STRUCTURES:
//    - set<string>            → currently visited nodes (used + un-used)
//    - vector<string>         → current path being built
//    - vector<pair<...>>      → all completed paths found
//
//  TIME COMPLEXITY: O(V!) worst case (all permutations)

void CampusGraph::dfsHelper(
    const string& current, const string& dest,
    bool avoidStairs, bool avoidNarrow,
    set<string>& visited, vector<string>& path, int dist,
    vector<pair<vector<string>, int>>& allPaths)
{
    // Base case: reached destination — save this path
    if (current == dest) {
        allPaths.push_back({path, dist});
        return;
    }

    // Try every neighbour
    for (const Edge& edge : adjList[current]) {
        if (visited.count(edge.to) > 0) continue;  // avoid cycles
        if (!canUse(edge, avoidStairs, avoidNarrow)) continue;

        // VISIT: add to path
        visited.insert(edge.to);
        path.push_back(edge.to);

        // Recurse deeper
        dfsHelper(edge.to, dest, avoidStairs, avoidNarrow,
                  visited, path, dist + edge.distance, allPaths);

        // BACKTRACK: remove from path so other routes can use this node
        path.pop_back();
        visited.erase(edge.to);
    }
}

vector<pair<vector<string>, int>> CampusGraph::dfsAllPaths(
    const string& start, const string& end,
    bool avoidStairs, bool avoidNarrow)
{
    vector<pair<vector<string>, int>> allPaths;
    if (!locations.count(start) || !locations.count(end))
        return allPaths;

    set<string> visited;
    vector<string> path;
    visited.insert(start);
    path.push_back(start);

    dfsHelper(start, end, avoidStairs, avoidNarrow,
              visited, path, 0, allPaths);

    // Sort paths by distance (shortest first)
    sort(allPaths.begin(), allPaths.end(),
         [](const auto& a, const auto& b) { return a.second < b.second; });

    return allPaths;
}

// ════════════════════════════════════════════════════════════
//  DIJKSTRA — Shortest Distance Route
// ════════════════════════════════════════════════════════════
//
//  HOW IT WORKS:
//    1. Set distance to start = 0, all others = infinity.
//    2. Always pick the CLOSEST unvisited node (using min-heap).
//    3. Update distances to its neighbours if a shorter path is found.
//    4. Repeat until destination is reached or all nodes checked.
//
//  DATA STRUCTURES:
//    - priority_queue (min-heap) → pick closest node efficiently
//    - map<string, int>         → shortest known distance to each node
//    - map<string, string>      → parent of each node (reconstruct path)
//
//  TIME COMPLEXITY: O((V + E) log V)

pair<vector<string>, int> CampusGraph::dijkstraRoute(
    const string& start, const string& end,
    bool avoidStairs, bool avoidNarrow)
{
    if (!locations.count(start) || !locations.count(end))
        return {{}, -1};
    if (start == end) return {{start}, 0};

    // Distance table: initially all infinity
    map<string, int> dist;
    map<string, string> parent;
    for (const string& loc : locations) dist[loc] = INT_MAX;

    dist[start] = 0;
    parent[start] = "";

    // Min-heap: (distance, location name)
    // greater<> makes it a MIN priority queue (smallest distance first)
    priority_queue<pair<int, string>,
                   vector<pair<int, string>>,
                   greater<pair<int, string>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        pair<int, string> entry = pq.top();
        pq.pop();
        int d = entry.first;
        string current = entry.second;

        if (d > dist[current]) continue;  // stale entry, skip
        if (current == end) break;        // destination reached

        for (const Edge& edge : adjList[current]) {
            if (!canUse(edge, avoidStairs, avoidNarrow)) continue;

            int newDist = dist[current] + edge.distance;
            // Found a SHORTER path to this neighbour
            if (newDist < dist[edge.to]) {
                dist[edge.to] = newDist;
                parent[edge.to] = current;
                pq.push({newDist, edge.to});
            }
        }
    }

    if (dist[end] == INT_MAX) return {{}, -1}; // unreachable

    // Reconstruct path
    vector<string> route;
    string node = end;
    while (node != "") {
        route.push_back(node);
        node = parent[node];
    }
    reverse(route.begin(), route.end());

    return {route, dist[end]};
}

// ════════════════════════════════════════════════════════════
//  FIND UNREACHABLE LOCATIONS
// ════════════════════════════════════════════════════════════
//
//  Uses BFS to find all reachable locations from a start point.
//  Any location NOT reached is unreachable under current constraints.

vector<string> CampusGraph::findUnreachable(
    const string& start, bool avoidStairs, bool avoidNarrow)
{
    set<string> reachable;
    queue<string> q;
    q.push(start);

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        if (reachable.count(current)) continue;
        reachable.insert(current);

        for (const Edge& edge : adjList[current]) {
            if (canUse(edge, avoidStairs, avoidNarrow))
                q.push(edge.to);
        }
    }

    vector<string> unreachable;
    for (const string& loc : locations) {
        if (!reachable.count(loc))
            unreachable.push_back(loc);
    }
    return unreachable;
}
