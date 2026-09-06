// graph.h - Campus Accessibility Route Finder
// A small graph for a DSA project.
//
// The campus is stored as an adjacency list:
//   for each location we keep a list of edges to its neighbours.
//
// Three search algorithms are implemented:
//   1. BFS      - route with the fewest stops
//   2. DFS      - every possible route (backtracking)
//   3. Dijkstra - route with the shortest total distance

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>

using namespace std;

// One edge (corridor) between two locations.
struct Edge {
    string to;         // destination location
    int    distance;   // metres
    bool   hasStairs;  // true if this corridor uses stairs
};

// The campus graph. Undirected, so every edge is stored twice.
class CampusGraph {
private:
    // adjacency list: location -> all edges leaving it
    map<string, vector<Edge>> adjList;
    // all location names (kept sorted for a neat display)
    set<string> locations;

    // Can this edge be used by the current profile?
    // Wheelchair mode (avoidStairs) skips any edge that has stairs.
    bool canUse(const Edge& edge, bool avoidStairs) const;

    // distance between two directly connected locations
    int getDistance(const string& from, const string& to) const;

    // recursive helper for DFS
    void dfsHelper(const string& current,
                   const string& destination,
                   bool avoidStairs,
                   set<string>& visited,
                   vector<string>& path,
                   int dist,
                   vector<pair<vector<string>, int>>& allPaths);

public:
    void addLocation(const string& name);
    void addPath(const string& from, const string& to, int distance, bool hasStairs = false);
    void displayGraph() const;

    // BFS - fewest stops. Returns the route and total distance.
    pair<vector<string>, int> bfsRoute(const string& start, const string& end, bool avoidStairs);

    // DFS - all paths via backtracking, sorted by distance.
    vector<pair<vector<string>, int>> dfsExplore(const string& start, const string& end, bool avoidStairs);

    // Dijkstra - shortest distance.
    pair<vector<string>, int> dijkstraRoute(const string& start, const string& end, bool avoidStairs);
};

#endif
