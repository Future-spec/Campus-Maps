#include "graph.h"

#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>

using namespace std;

CampusGraph::CampusGraph(const vector<string>& locationNames)
    : locations(locationNames), adjList(locationNames.size()) {}

bool CampusGraph::isValidLocation(int location) const {
    return location >= 0 && location < locationCount();
}

// The single accessibility rule: wheelchair mode does not use stairs.
bool CampusGraph::canUse(const Edge& edge, bool wheelchairMode) const {
    return !(wheelchairMode && edge.hasStairs);
}

void CampusGraph::addPath(int from, int to, int distance, bool hasStairs) {
    if (!isValidLocation(from) || !isValidLocation(to) || distance <= 0) return;
    adjList[from].push_back({to, distance, hasStairs});
    adjList[to].push_back({from, distance, hasStairs});
}

void CampusGraph::displayLocations() const {
    cout << "\nLocations:\n";
    for (int i = 0; i < locationCount(); ++i) {
        cout << "  " << i << ". " << locations[i] << "\n";
    }
}

void CampusGraph::displayGraph() const {
    cout << "\nAdjacency List:\n";
    for (int from = 0; from < locationCount(); ++from) {
        cout << locations[from] << " -> ";
        for (const Edge& edge : adjList[from]) {
            cout << locations[edge.to] << " (" << edge.distance << "m";
            if (edge.hasStairs) cout << ", stairs";
            cout << ")  ";
        }
        cout << "\n";
    }
}

void CampusGraph::dfsHelper(int current, bool wheelchairMode,
                            vector<bool>& visited, vector<int>& order) const {
    visited[current] = true;
    order.push_back(current);
    for (const Edge& edge : adjList[current]) {
        if (!visited[edge.to] && canUse(edge, wheelchairMode)) {
            dfsHelper(edge.to, wheelchairMode, visited, order);
        }
    }
}

vector<int> CampusGraph::dfsTraversal(int start, bool wheelchairMode) const {
    if (!isValidLocation(start)) return {};
    vector<bool> visited(locationCount(), false);
    vector<int> order;
    dfsHelper(start, wheelchairMode, visited, order);
    return order;
}

// BFS uses a queue. The first route found has the fewest edges/stops.
vector<int> CampusGraph::bfsRoute(int start, int end, bool wheelchairMode) const {
    if (!isValidLocation(start) || !isValidLocation(end)) return {};

    queue<int> q;
    vector<bool> visited(locationCount(), false);
    vector<int> parent(locationCount(), -1);
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        if (current == end) break;

        for (const Edge& edge : adjList[current]) {
            if (visited[edge.to] || !canUse(edge, wheelchairMode)) continue;
            visited[edge.to] = true;
            parent[edge.to] = current;
            q.push(edge.to);
        }
    }

    if (!visited[end]) return {};
    vector<int> route;
    for (int current = end; current != -1; current = parent[current]) {
        route.push_back(current);
    }
    reverse(route.begin(), route.end());
    return route;
}

// Dijkstra uses a min-heap. It minimises total distance, not number of stops.
vector<int> CampusGraph::dijkstraRoute(int start, int end, bool wheelchairMode) const {
    if (!isValidLocation(start) || !isValidLocation(end)) return {};

    vector<int> distance(locationCount(), INT_MAX);
    vector<int> parent(locationCount(), -1);
    priority_queue<pair<int, int>, vector<pair<int, int> >,
                   greater<pair<int, int> > > minHeap;
    distance[start] = 0;
    minHeap.push(make_pair(0, start));

    while (!minHeap.empty()) {
        pair<int, int> top = minHeap.top();
        minHeap.pop();
        int currentDistance = top.first;
        int current = top.second;
        if (currentDistance != distance[current]) continue;
        if (current == end) break;

        for (const Edge& edge : adjList[current]) {
            if (!canUse(edge, wheelchairMode)) continue;
            int newDistance = currentDistance + edge.distance;
            if (newDistance < distance[edge.to]) {
                distance[edge.to] = newDistance;
                parent[edge.to] = current;
                minHeap.push(make_pair(newDistance, edge.to));
            }
        }
    }

    if (distance[end] == INT_MAX) return {};
    vector<int> route;
    for (int current = end; current != -1; current = parent[current]) {
        route.push_back(current);
    }
    reverse(route.begin(), route.end());
    return route;
}

int CampusGraph::routeDistance(const vector<int>& route) const {
    int total = 0;
    for (size_t i = 0; i + 1 < route.size(); ++i) {
        for (const Edge& edge : adjList[route[i]]) {
            if (edge.to == route[i + 1]) {
                total += edge.distance;
                break;
            }
        }
    }
    return total;
}

const string& CampusGraph::getLocationName(int location) const {
    return locations[location];
}

int CampusGraph::locationCount() const {
    return static_cast<int>(locations.size());
}
