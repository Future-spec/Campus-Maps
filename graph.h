// Small graph ADT: vertex = location, edge = corridor, weight = metres.

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

struct Edge {
    int to;
    int distance;
    bool hasStairs;
};

class CampusGraph {
private:
    std::vector<std::string> locations;
    std::vector<std::vector<Edge> > adjList;

    bool canUse(const Edge& edge, bool wheelchairMode) const;
    bool isValidLocation(int location) const;
    void dfsHelper(int current, bool wheelchairMode,
                   std::vector<bool>& visited, std::vector<int>& order) const;

public:
    explicit CampusGraph(const std::vector<std::string>& locationNames);

    // One undirected edge is stored twice: A -> B and B -> A.
    void addPath(int from, int to, int distance, bool hasStairs = false);
    void displayLocations() const;
    void displayGraph() const;

    std::vector<int> dfsTraversal(int start, bool wheelchairMode) const;
    std::vector<int> bfsRoute(int start, int end, bool wheelchairMode) const;
    std::vector<int> dijkstraRoute(int start, int end, bool wheelchairMode) const;

    int routeDistance(const std::vector<int>& route) const;
    const std::string& getLocationName(int location) const;
    int locationCount() const;
};

#endif
