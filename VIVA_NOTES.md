# Four-person viva notes

Keep the explanation simple. Do not memorise every line of code; understand the idea of each part.

## Member 1 - Problem and graph

"Our problem is to find a route on a small campus map. Each place, such as Entrance or Library, is a vertex. Each corridor is an edge. The edge has a distance in metres and can be marked as stairs. Our graph is undirected because a person can walk in both directions."

"We used an adjacency list: for every vertex, we store only its neighbouring vertices. Campus maps are sparse, so this uses less space than an adjacency matrix."

## Member 2 - DFS

"DFS means Depth-First Search. It visits one neighbour, continues as far as possible, then backtracks. We use recursion and a visited array so that no location is visited twice. In this project DFS shows the order in which reachable locations are explored."

"Time complexity is O(V + E), where V is the number of vertices and E is the number of edges."

## Member 3 - BFS

"BFS means Breadth-First Search. It uses a queue. It first checks all nearby vertices, then vertices one level farther away. Therefore, in an unweighted graph, BFS gives the route with the fewest edges or stops."

"We store the parent of each visited location. Once we reach the destination, we follow parent values backwards to print the route. BFS also takes O(V + E)."

## Member 4 - Dijkstra and accessibility

"Our edges have different distances, so fewest stops is not always the shortest walk. Dijkstra's algorithm keeps the smallest current distance first using a min-priority queue. When a shorter route is found, it updates distance and parent."

"Wheelchair mode is our accessibility condition. Before DFS, BFS or Dijkstra uses an edge, the `canUse` function checks whether it has stairs. If wheelchair mode is on, stair edges are skipped."

"Dijkstra's time complexity with a priority queue is O((V + E) log V)."

## The one demo to remember

From **Entrance (0)** to **Seminar Hall (6)** with wheelchair mode off:

- BFS: `Entrance -> Main Corridor -> Seminar Hall`, 2 stops, 60 m.
- Dijkstra: `Entrance -> Main Corridor -> Stairs -> Seminar Hall`, 3 stops, 25 m.

This proves the difference: **BFS minimises stops; Dijkstra minimises distance.**

Turn wheelchair mode on and Dijkstra avoids Stairs, then uses Lift instead.

## Likely questions

**Why not an adjacency matrix?** Our graph has few corridors compared to all possible pairs of locations. An adjacency list uses O(V + E) space; a matrix uses O(V^2).

**Why are corridors stored twice?** The graph is undirected. A corridor can be used in both directions.

**Why use a visited array?** It prevents cycles and repeated work.

**Why can BFS give a longer distance?** BFS does not look at metres; it only counts how many edges/stops are used.
