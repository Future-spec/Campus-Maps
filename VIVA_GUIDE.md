# Viva Guide: Campus Accessibility Route Finder

## One-line explanation

This project represents a campus as a graph and finds routes that respect accessibility requirements such as avoiding stairs or blocked corridors.

## Four-member division

### Member 1: Problem and data model
- Explain the real problem: a normal shortest route may not be usable by every student.
- Explain a graph: rooms are vertices and corridors are edges.
- Explain that each edge stores distance and accessibility information.

### Member 2: BFS and DFS
- BFS visits nearby locations level by level.
- BFS is useful when the goal is the fewest number of stops.
- DFS explores paths deeply and backtracks to find alternative routes.

### Member 3: Dijkstra and accessibility
- Dijkstra chooses the currently cheapest distance and updates better distances.
- Before using an edge, the program checks stairs, narrow paths, lifts, and blocked corridors.
- The same graph can produce different answers for different accessibility profiles.

### Member 4: Interface and demonstration
- Explain the console menu and the browser map.
- Show selecting a profile, choosing start and destination, and comparing algorithms.
- Click a corridor to block it, then run the route again to show the result changes.

## Five-minute demonstration

1. Start with the console version or open the browser map.
2. Select a wheelchair profile.
3. Find a route from Main Entrance to 213 Computer Centre.
4. Compare BFS and Dijkstra.
5. Block one corridor and run the route again.
6. Briefly show DFS as an alternative-path search.

## Simple viva answers

**Why use BFS?** BFS finds a route with the fewest edges in an unweighted graph.

**Why use Dijkstra?** Corridors have different distances, so Dijkstra finds the shortest total distance.

**Why not use only Dijkstra?** Different algorithms answer different questions: fewest stops versus shortest distance.

**What is the time complexity?** BFS and DFS are $O(V + E)$. The simple teaching implementation of Dijkstra is $O((V + E)V)$ because it scans an array for the smallest distance.

**What is the limitation?** The map is sample data and the current app does not use live campus maps or GPS.

## Keep deployment secondary

The `web/` folder is a browser demonstration and can be hosted on Vercel. The main academic explanation is the graph, accessibility filtering, and algorithms.
