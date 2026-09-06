// graph.cpp - BFS, DFS and Dijkstra implementations

#include "graph.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

// --- helpers ---------------------------------------------------------------

bool CampusGraph::canUse(const Edge& edge, bool avoidStairs) const {
    // wheelchair mode: skip any corridor that needs stairs
    if (avoidStairs && edge.hasStairs) return false;
    return true;
}

int CampusGraph::getDistance(const string& from, const string& to) const {
    auto it = adjList.find(from);
    if (it == adjList.end()) return -1;
    for (const Edge& e : it->second) {
        if (e.to == to) return e.distance;
    }
    return -1;
}

// --- location management ---------------------------------------------------

void CampusGraph::addLocation(const string& name) {
    if (locations.count(name) > 0) return;   // already present
    locations.insert(name);
    adjList[name];                            // give it an empty edge list
}

// --- edge management -------------------------------------------------------

// Undirected graph: the edge is stored in both directions.
void CampusGraph::addPath(const string& from, const string& to, int distance, bool hasStairs) {
    addLocation(from);
    addLocation(to);

    adjList[from].push_back({to, distance, hasStairs});
    adjList[to].push_back({from, distance, hasStairs});

    cout << "  [+] " << from << " <--(" << distance << "m)--> " << to;
    if (hasStairs) cout << "  [Stairs]";
    cout << "\n";
}

// --- display ---------------------------------------------------------------

void CampusGraph::displayGraph() const {
    cout << "\n  --- CAMPUS MAP (Adjacency List) ---\n";
    if (locations.empty()) {
        cout << "    (no locations)\n\n";
        return;
    }
    for (const string& loc : locations) {
        cout << "  [" << loc << "]\n";
        auto it = adjList.find(loc);
        if (it == adjList.end() || it->second.empty()) {
            cout << "      (no connections)\n";
            continue;
        }
        for (const Edge& e : it->second) {
            cout << "      --> " << left << setw(18) << e.to
                 << right << setw(5) << e.distance << "m";
            if (e.hasStairs) cout << "  [Stairs]";
            cout << "\n";
        }
    }
    cout << "\n  Total locations: " << locations.size() << "\n\n";
}

// --- BFS: fewest stops -----------------------------------------------------
// Uses a queue and explores level by level, so the first time we reach
// the destination is guaranteed to be via the fewest number of edges/stops.
// Time complexity: O(V + E)

pair<vector<string>, int> CampusGraph::bfsRoute(const string& start,
                                                const string& end,
                                                bool avoidStairs) {
    if (locations.count(start) == 0 || locations.count(end) == 0) {
        return {{}, -1};
    }
    if (start == end) return {{start}, 0};

    queue<string> q;
    set<string> visited;
    map<string, string> parent;   // parent[x] = how we first reached x

    q.push(start);
    visited.insert(start);
    parent[start] = "";

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        for (const Edge& edge : adjList[current]) {
            if (visited.count(edge.to) > 0) continue;   // already seen
            if (!canUse(edge, avoidStairs)) continue;   // inaccessible

            visited.insert(edge.to);
            parent[edge.to] = current;

            // reached the destination - reconstruct the route
            if (edge.to == end) {
                vector<string> route;
                string node = end;
                while (node != "") {
                    route.push_back(node);
                    node = parent[node];
                }
                reverse(route.begin(), route.end());

                int total = 0;
                for (size_t i = 0; i + 1 < route.size(); i++) {
                    total += getDistance(route[i], route[i + 1]);
                }
                return {route, total};
            }

            q.push(edge.to);
        }
    }

    return {{}, -1};   // no route found
}

// --- DFS: all paths (backtracking) ----------------------------------------
// Goes as deep as possible along one branch, then backtracks to try another.
// We unvisit a node after exploring it so it can appear on other routes.

void CampusGraph::dfsHelper(const string& current,
                            const string& destination,
                            bool avoidStairs,
                            set<string>& visited,
                            vector<string>& path,
                            int dist,
                            vector<pair<vector<string>, int>>& allPaths) {
    if (current == destination) {
        allPaths.push_back({path, dist});
        return;
    }

    for (const Edge& edge : adjList[current]) {
        if (visited.count(edge.to) > 0) continue;   // avoid cycles
        if (!canUse(edge, avoidStairs)) continue;

        visited.insert(edge.to);
        path.push_back(edge.to);
        dfsHelper(edge.to, destination, avoidStairs, visited, path,
                  dist + edge.distance, allPaths);
        path.pop_back();          // backtrack
        visited.erase(edge.to);   // allow this node on other routes
    }
}

vector<pair<vector<string>, int>> CampusGraph::dfsExplore(const string& start,
                                                          const string& end,
                                                          bool avoidStairs) {
    vector<pair<vector<string>, int>> allPaths;
    if (locations.count(start) == 0 || locations.count(end) == 0) return allPaths;

    set<string> visited;
    vector<string> path;
    visited.insert(start);
    path.push_back(start);

    dfsHelper(start, end, avoidStairs, visited, path, 0, allPaths);

    // least distance first
    sort(allPaths.begin(), allPaths.end(),
         [](const pair<vector<string>, int>& a,
            const pair<vector<string>, int>& b) {
             return a.second < b.second;
         });
    return allPaths;
}

// --- Dijkstra: shortest distance ------------------------------------------
// Uses a min-priority queue so the closest unvisited node is expanded first.
// Time complexity: O((V + E) log V)

pair<vector<string>, int> CampusGraph::dijkstraRoute(const string& start,
                                                     const string& end,
                                                     bool avoidStairs) {
    if (locations.count(start) == 0 || locations.count(end) == 0) {
        return {{}, -1};
    }
    if (start == end) return {{start}, 0};

    // min-heap of (distance, location)
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    map<string, int> dist;
    map<string, string> parent;

    for (const string& loc : locations) {
        dist[loc] = INT_MAX;   // "infinity"
    }
    dist[start] = 0;
    parent[start] = "";
    pq.push({0, start});

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        string current = top.second;

        if (d > dist[current]) continue;   // stale entry, skip it
        if (current == end) break;         // destination reached

        for (const Edge& edge : adjList[current]) {
            if (!canUse(edge, avoidStairs)) continue;

            int newDist = dist[current] + edge.distance;
            if (newDist < dist[edge.to]) {
                dist[edge.to] = newDist;
                parent[edge.to] = current;
                pq.push({newDist, edge.to});
            }
        }
    }

    if (dist[end] == INT_MAX) return {{}, -1};   // unreachable

    vector<string> route;
    string node = end;
    while (node != "") {
        route.push_back(node);
        node = parent[node];
    }
    reverse(route.begin(), route.end());

    return {route, dist[end]};
}
