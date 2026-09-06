# Campus Accessibility Route Finder

DSA course project - finds accessible routes in a campus building using graph algorithms.

We modelled the TCET 2nd floor (A-Wing, B-Wing, C-Wing) as a graph where rooms are nodes and corridors are edges.

## Features
- Find shortest route using Dijkstra's algorithm
- Find route with fewest stops using BFS
- Find all possible routes using DFS with backtracking
- Accessibility filtering (wheelchair, visually impaired)
- Block/unblock paths
- Interactive web visualisation

## DSA Concepts Used
- Graph (Adjacency List representation)
- BFS (Breadth First Search)
- DFS (Depth First Search) with backtracking
- Dijkstra's Algorithm (min-heap / priority queue)

## How to Run

### C++ (Console)
```
g++ -std=c++17 main.cpp graph.cpp -o campus_route_finder
./campus_route_finder
```

### Web Version
Open `web/index.html` in any browser.

## Files
- `graph.h` - Graph class definition
- `graph.cpp` - BFS, DFS, Dijkstra implementation
- `main.cpp` - Menu driven program with TCET campus data
- `web/` - Browser version with interactive map

## Team
Group project for DSA course, 2nd Year IT, TCET.
