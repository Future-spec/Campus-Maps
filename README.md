# Campus Accessibility Route Finder

DSA project: find **accessible routes** on a campus graph. The map is the **TCET 2nd floor** (A-Wing, B-Wing, C-Wing) from the college AutoCAD layout.

- **C++ console app** — what you compile and demo in the lab
- **Web app** (`web/`) — same algorithms in the browser, ready for **Vercel**

## Problem

Some corridors have stairs, narrow passages, or lifts. Given a user profile (wheelchair, visually impaired, or none), the system must:

1. Return an accessible route
2. List locations that cannot be reached
3. Compare alternative traversals (BFS vs DFS vs Dijkstra)

## DSA used

| Topic | How it is used |
| --- | --- |
| Graph + adjacency list | Rooms = vertices, corridors = undirected weighted edges |
| BFS | Fewest **stops** (queue, O(V+E)) |
| DFS + backtracking | All accessible paths (capped at 30 / 20) |
| Dijkstra | Shortest **distance** (min-heap, O((V+E) log V)) |

Edges are **filtered at traversal time**. Blocked / stairs / narrow / lift flags stay on the edge; the graph is not rebuilt.

## TCET 2nd-floor model

```
                    ┌────────────── A-WING ──────────────┐
                    │ IT staff · computer labs · lifts   │
                    └───────────────┬────────────────────┘
                                    │ Central Junction
                    ┌──────── B-WING ┼──── C-WING ────────┐
                    │ Guest / halls  │  210–214, seminar  │
                    │ Auditorium     │  Computer Centre   │
                    └────────────────┴────────────────────┘
```

- **A-Wing:** IT Department, staff room, System / Database / OS / Cloud / Project / Software labs, ladies & handicap restroom, fire lift, stairs
- **B-Wing:** Guest room, multipurpose hall, auditorium, rest room, fire lift, stairs
- **C-Wing:** 210 EXTC Lab-8 through 214, Seminar Hall III, Computer Centre, gents room, fire lift, stairs
- **Central:** junction, main lift, lobby stairs

## What to say in the viva

1. **Why adjacency list?** Campus graphs are sparse. List uses O(V+E) space; a matrix would be O(V²).
2. **BFS vs Dijkstra (run this):** `210 EXTC Lab-8` → `213 Computer Centre` with no constraints. BFS takes the C-Wing corridor (2 stops, 58 m). Dijkstra walks 210 → 211 → 212 → 213 (3 stops, 45 m).
3. **Wheelchair:** stairs are skipped; lifts are allowed. `Central Junction` → `Auditorium Hall` uses **B-Wing Fire Lift**. Click that lift corridor to **block** it — the hall is then unreachable (only stairs remain).
4. **Unreachable** is BFS reachability from the start node.

## C++ (lab / presentation)

```bash
g++ -std=c++17 main.cpp graph.cpp -o campus_route_finder
./campus_route_finder
```

| File | Role |
| --- | --- |
| `graph.h` | `Path`, constraints, `CampusGraph` |
| `graph.cpp` | BFS, DFS, Dijkstra, block/mark edges |
| `main.cpp` | Menu + TCET map |

## Web (Vercel)

Static site in `web/`: `index.html`, `css/style.css`, `js/graph.js`, `js/campus.js`, `js/app.js`.

### Deploy

1. Push this repo to GitHub
2. [vercel.com/new](https://vercel.com/new) → import the repo
3. Framework: **Other**. Output directory: `web`. Install/build: leave empty
4. Deploy

Or CLI:

```bash
npm i -g vercel
vercel
```

When prompted, set output directory to `web`.

## File structure

```
CampusAccessibilityRouteFinder/
  graph.h / graph.cpp / main.cpp   C++ DSA core
  web/                             Vercel static app
  vercel.json
  README.md
```
