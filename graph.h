/*
 * ============================================================
 *  graph.h — Campus Accessibility Route Finder
 * ============================================================
 *  DSA Concepts Used:
 *    - Graph representation using Adjacency List
 *    - BFS (Breadth-First Search) for fewest-stop routes
 *    - DFS (Depth-First Search) with backtracking for all paths
 *    - Dijkstra's Algorithm for shortest-distance routes
 *
 *  Why Adjacency List over Adjacency Matrix?
 *    - A campus graph is SPARSE (not every building connects
 *      to every other building).
 *    - Adjacency List uses O(V + E) space, vs O(V^2) for matrix.
 *    - Iterating over neighbors is O(degree) vs O(V).
 * ============================================================
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <utility>

using namespace std;

// ──────────────────────────────────────────────────────────────
//  STRUCT: Path (Edge in the graph)
// ──────────────────────────────────────────────────────────────
//  Each Path represents a BIDIRECTIONAL connection between two
//  campus locations, with a distance (weight) and accessibility
//  attributes.
//
//  In graph terms:
//    - 'from' and 'to' are the two VERTICES connected by this edge
//    - 'distance' is the EDGE WEIGHT
//    - The boolean flags are EDGE ATTRIBUTES for filtering
// ──────────────────────────────────────────────────────────────
struct Path {
    string from;
    string to;
    int    distance;

    bool   hasStairs;
    bool   isNarrow;
    bool   liftRequired;
    bool   isBlocked;
};

// ──────────────────────────────────────────────────────────────
//  STRUCT: AccessibilityConstraints
// ──────────────────────────────────────────────────────────────
//  Represents what a user CANNOT navigate.
//  If avoidStairs = true, the user cannot use paths with stairs.
//  This struct is passed to BFS/DFS/Dijkstra to filter edges.
// ──────────────────────────────────────────────────────────────
struct AccessibilityConstraints {
    bool avoidStairs       = false;
    bool avoidNarrow       = false;
    bool avoidLiftRequired = false;
};

// ──────────────────────────────────────────────────────────────
//  CLASS: CampusGraph
// ──────────────────────────────────────────────────────────────
//  The main graph ADT using an ADJACENCY LIST.
//
//  Internal representation:
//    unordered_map<string, vector<Path>>
//      Key   = location name (vertex)
//      Value = list of paths (edges) from that location
//
//  This is an UNDIRECTED, WEIGHTED graph:
//    Each edge is stored twice (once for each direction)
//    Edge weights are distances in metres
// ──────────────────────────────────────────────────────────────
class CampusGraph {
private:
    unordered_map<string, vector<Path>> adjList;
    set<string> locations;

    bool isAccessible(const Path& path,
                      const AccessibilityConstraints& constraints) const;

    int getEdgeDistance(const string& from, const string& to) const;

    void dfsHelper(const string& current,
                   const string& destination,
                   const AccessibilityConstraints& constraints,
                   set<string>& visited,
                   vector<string>& currentPath,
                   int currentDist,
                   vector<pair<vector<string>, int>>& allPaths);

public:
    void addLocation(const string& name);
    bool locationExists(const string& name) const;
    const set<string>& getLocations() const;

    void addPath(const string& from, const string& to, int distance,
                 bool hasStairs    = false,
                 bool isNarrow     = false,
                 bool liftRequired = false,
                 bool silent       = false);
    bool removePath(const string& from, const string& to);
    void blockPath(const string& from, const string& to);
    void unblockPath(const string& from, const string& to);
    void markAccessibility(const string& from, const string& to,
                           bool hasStairs, bool isNarrow, bool liftRequired);

    // BFS: fewest stops. O(V + E)
    pair<vector<string>, int> bfsRoute(
        const string& start, const string& end,
        const AccessibilityConstraints& constraints);

    // DFS: all paths via backtracking. O(V!) worst case
    vector<pair<vector<string>, int>> dfsExplore(
        const string& start, const string& end,
        const AccessibilityConstraints& constraints);

    // Dijkstra: shortest distance. O((V+E) log V)
    pair<vector<string>, int> dijkstraRoute(
        const string& start, const string& end,
        const AccessibilityConstraints& constraints);

    void compareRoutes(const string& start, const string& end,
                       const AccessibilityConstraints& constraints);

    vector<string> showInaccessible(const string& start,
                                    const AccessibilityConstraints& constraints);

    void displayGraph() const;
};

#endif
