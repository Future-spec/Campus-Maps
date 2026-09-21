// graph.h — Campus Accessibility Route Finder
//
// DATA STRUCTURES USED:
//   - Adjacency List: map<string, vector<Edge>>
//     Each location stores a list of edges (corridors) to its neighbours.
//   - set<string> for unique location names (sorted order).
//
// WHY ADJACENCY LIST?
//   A campus has few corridors compared to all possible pairs.
//   Adjacency list uses O(V + E) space instead of O(V²) for a matrix.

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

using namespace std;

// ── One corridor between two locations ──────────────────────
struct Edge {
    string to;          // destination location
    int    distance;    // in metres
    bool   hasStairs;   // true = wheelchair cannot use this path
    bool   isNarrow;    // true = wheelchair cannot use this path
    bool   isBlocked;   // true = path is temporarily blocked
};

// ── The campus graph ────────────────────────────────────────
class CampusGraph {
private:
    // ADJACENCY LIST: location name → list of edges leaving it
    map<string, vector<Edge>> adjList;

    // All location names (sorted set for neat display)
    set<string> locations;

    // Check if an edge can be used with current accessibility constraints
    bool canUse(const Edge& edge, bool avoidStairs, bool avoidNarrow) const;

    // Get distance between two directly connected locations
    int getDistance(const string& from, const string& to) const;

    // Recursive helper for DFS (backtracking)
    void dfsHelper(const string& current, const string& dest,
                   bool avoidStairs, bool avoidNarrow,
                   set<string>& visited, vector<string>& path, int dist,
                   vector<pair<vector<string>, int>>& allPaths);

public:
    // ── Build the graph ─────────────────────────────────────
    void addLocation(const string& name);
    void addPath(const string& from, const string& to, int distance,
                 bool hasStairs = false, bool isNarrow = false);
    void blockPath(const string& from, const string& to);
    void unblockPath(const string& from, const string& to);
    void displayGraph() const;

    // ── Route finding algorithms ────────────────────────────

    // BFS: finds route with FEWEST STOPS (edges)
    // Time: O(V + E)
    pair<vector<string>, int> bfsRoute(const string& start, const string& end,
                                       bool avoidStairs, bool avoidNarrow);

    // DFS: finds ALL possible paths using backtracking
    // Time: O(V! in worst case) — explores every permutation
    vector<pair<vector<string>, int>> dfsAllPaths(
        const string& start, const string& end,
        bool avoidStairs, bool avoidNarrow);

    // Dijkstra: finds route with SHORTEST DISTANCE (metres)
    // Time: O((V + E) log V) with priority queue
    pair<vector<string>, int> dijkstraRoute(const string& start, const string& end,
                                            bool avoidStairs, bool avoidNarrow);

    // Find locations unreachable from a given start point
    vector<string> findUnreachable(const string& start,
                                   bool avoidStairs, bool avoidNarrow);

    // Getter for location names
    const set<string>& getLocations() const { return locations; }
};

#endif
