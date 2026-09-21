# Campus Accessibility Route Finder

DSA project — finds accessible routes in a 3-floor campus building using BFS, DFS and Dijkstra.

## Campus Layout

```
Floor 3:  [Seminar Hall] ---- [Physics Lab] ---- [Principal Office]
                |                                        |
             (Stairs)                                  (Lift)
                |                                        |
Floor 2:  [Computer Lab] --- [Classroom 201] ------ [Staff Room]
                |                  |                      |
             (Stairs)         (Narrow Path)             (Lift)
                |                  |                      |
Floor 1:   [Main Gate] ------- [Library] ----------- [Canteen]
```

- 9 locations across 3 floors
- 13 bidirectional paths
- Stairs, narrow passages, and lift connections between floors

## How to Run (VS Code Terminal)

```powershell
g++ -std=c++17 main.cpp graph.cpp -o campus.exe
.\campus.exe
```

No external libraries needed. Just C++ standard library.

## Menu Options

```
1. Display Campus Map (Adjacency List)
2. Find Route — BFS (Fewest Stops)
3. Find Route — Dijkstra (Shortest Distance)
4. Explore All Paths — DFS (Backtracking)
5. Compare Routes (BFS vs Dijkstra)
6. Show Unreachable Locations
7. Block a Path
8. Unblock a Path
9. Toggle Wheelchair Mode
0. Exit
```

## DSA Concepts Used

| Concept | Where | Why |
|---------|-------|-----|
| **Graph** | `adjList` in graph.h | Campus is nodes (rooms) + edges (corridors) |
| **Adjacency List** | `map<string, vector<Edge>>` | Sparse graph — O(V+E) space vs O(V²) matrix |
| **BFS** | `bfsRoute()` in graph.cpp | Finds route with fewest stops — uses queue |
| **DFS** | `dfsAllPaths()` in graph.cpp | Finds ALL routes via backtracking — uses recursion |
| **Dijkstra** | `dijkstraRoute()` in graph.cpp | Finds shortest distance — uses min-heap |

## Key Demo Scenarios

### Demo 1: Compare BFS vs Dijkstra (wheelchair OFF)
- Main Gate → Principal Office
- BFS and Dijkstra may find different routes when distances vary

### Demo 2: Wheelchair mode ON
- Toggle wheelchair mode (option 9)
- Stairs and narrow paths are skipped
- Only lifts remain for going between floors
- Route must go through Canteen → Staff Room (lift)

### Demo 3: Block the lift, wheelchair ON
- Block Canteen ↔ Staff Room (option 7)
- Block Staff Room ↔ Principal Office (option 7)
- Now show unreachable locations (option 6)
- Floor 2 and Floor 3 become unreachable for wheelchair users

### Demo 4: DFS — all paths
- Run DFS from Main Gate → Principal Office
- Shows every possible route sorted by distance

## Files

| File | Lines | Purpose |
|------|-------|---------|
| `graph.h` | ~80 | Edge struct + CampusGraph class declaration |
| `graph.cpp` | ~270 | BFS, DFS, Dijkstra implementations with comments |
| `main.cpp` | ~200 | 3-floor campus setup + menu + formatted output |

## Team Division (4 Members)

### Member 1: Graph Data Structure
- Explain `graph.h`: Edge struct, adjacency list (`map<string, vector<Edge>>`)
- Why adjacency list over matrix (sparse graph, O(V+E) space)
- `addPath()`, `blockPath()`, `displayGraph()`

### Member 2: BFS (Breadth-First Search)
- Explain `bfsRoute()` in graph.cpp
- Uses queue (FIFO), visits level-by-level
- Finds route with fewest stops/edges
- Time: O(V + E)

### Member 3: DFS (Depth-First Search)
- Explain `dfsAllPaths()` and `dfsHelper()` in graph.cpp
- Uses recursion + backtracking
- Finds ALL possible routes between two points
- Visit → recurse → un-visit (backtrack)

### Member 4: Dijkstra + Accessibility
- Explain `dijkstraRoute()` in graph.cpp
- Uses min-heap (priority queue)
- Finds shortest total distance
- Explain `canUse()` — how accessibility filtering works
- Demo: compare BFS vs Dijkstra, wheelchair mode, block paths

## Viva Questions

**Q: Why use an adjacency list?**
Our campus has 9 locations and 13 paths. An adjacency matrix would waste space storing 81 cells (9×9) when only 26 are used (13 edges × 2 directions).

**Q: What's the difference between BFS and Dijkstra?**
BFS finds the route with fewest stops (edges). Dijkstra finds the route with shortest total distance (metres). BFS uses a queue, Dijkstra uses a priority queue.

**Q: How does DFS find all paths?**
DFS goes deep along one branch, records the path if it reaches the destination, then backtracks by un-visiting the last node and trying the next neighbour. This explores every possible combination.

**Q: How does wheelchair filtering work?**
Before BFS/DFS/Dijkstra uses an edge, `canUse()` checks if `hasStairs` or `isNarrow` is true. If wheelchair mode is on and the edge has stairs or is narrow, it's skipped.

**Q: Time complexity?**
BFS: O(V + E). DFS with backtracking: O(V!) worst case. Dijkstra with min-heap: O((V + E) log V).

**Q: What happens when you block all paths between floors?**
`findUnreachable()` uses BFS to mark all reachable nodes from the start. Any node not marked is unreachable. This shows which rooms a wheelchair user cannot access.
