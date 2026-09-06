# Campus Accessibility Route Finder

A small second-year DSA project. It finds routes in a campus graph while showing the difference between DFS, BFS and Dijkstra's algorithm.

The project intentionally uses a tiny map: **7 locations and 9 corridors**. That makes every vertex, edge and output easy to explain in a viva.

## DSA concepts used

| Concept | Where it is used |
| --- | --- |
| Graph | Campus locations are vertices and corridors are edges. |
| Adjacency list | `vector<vector<Edge>>` stores only the neighbours of each location. |
| DFS | Visits reachable places by going deep before backtracking. |
| BFS | Finds a route with the fewest stops using a queue. |
| Dijkstra | Finds the lowest total distance using a min-priority queue. |

The graph is undirected, so every corridor is stored twice: `A -> B` and `B -> A`.

## Accessibility rule

There is one clear rule: when **wheelchair mode** is on, edges marked as stairs are skipped. The rest of the graph remains the same. This is a simple example of filtering graph edges before traversal.

## Best viva demo

Use **Entrance (0)** as the start and **Seminar Hall (6)** as the destination.

With wheelchair mode off:

- BFS gives `Entrance -> Main Corridor -> Seminar Hall`: 2 stops, 60 m. BFS chooses fewer edges.
- Dijkstra gives `Entrance -> Main Corridor -> Stairs -> Seminar Hall`: 3 stops, 25 m. Dijkstra chooses fewer metres.

Turn wheelchair mode on, then run Dijkstra again:

- Stair edges are skipped, so it uses `Entrance -> Main Corridor -> Lift -> Seminar Hall`: 3 stops, 35 m.

## Run the C++ program

```bash
g++ -std=c++17 main.cpp graph.cpp -o campus_route_finder
./campus_route_finder
```

Windows PowerShell:

```powershell
g++ -std=c++17 main.cpp graph.cpp -o campus_route_finder.exe
.\campus_route_finder.exe
```

## Web demo

Open `web/index.html` in a browser, or deploy the `web` folder as a static site. The browser version mirrors the same 7-node graph and algorithms as the C++ program.

## Files

| File | Purpose |
| --- | --- |
| `main.cpp` | Small menu-driven demo and campus data. |
| `graph.h` | `Edge` structure and `CampusGraph` class declaration. |
| `graph.cpp` | Adjacency list, DFS, BFS and Dijkstra implementation. |
| `web/` | Simple visual version of the same program. |
| `VIVA_NOTES.md` | Short explanation split across four team members. |
