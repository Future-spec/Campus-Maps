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

## How to Run the Console Version

```powershell
g++ -std=c++17 main.cpp graph.cpp -o campus_route_finder
./campus_route_finder
```

The console program runs without a database or external library.

## How to Run the Browser Version

Open `web/index.html` in a modern browser, or serve the project folder with a local static server. The browser version demonstrates the same graph ideas with route selection, accessibility profiles, and visual feedback.

## Files
- `graph.h` - Graph class definition
- `graph.cpp` - BFS, DFS, Dijkstra implementation
- `main.cpp` - Menu driven program with TCET campus data
- `web/` - Browser version with interactive map
- `web/js/graph.js` - Browser graph algorithms
- `web/js/campus.js` - Browser campus nodes, edges, and profiles

## Viva Preparation

See [VIVA_GUIDE.md](VIVA_GUIDE.md) for a four-member division, a five-minute demonstration, and common viva questions.

## Team
Group project for DSA course, 2nd Year IT, TCET.
