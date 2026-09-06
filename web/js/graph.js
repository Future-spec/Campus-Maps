// graph.js - Graph class for the campus route finder.
// Mirrors the C++ version: adjacency list, BFS, DFS, Dijkstra,
// and a single accessibility rule (wheelchair mode avoids stairs).

export class CampusGraph {
  constructor() {
    this.adjList = new Map();    // location -> [ {to, distance, hasStairs} ]
    this.locations = new Set();  // set of all location names
  }

  addLocation(name) {
    if (this.locations.has(name)) return;
    this.locations.add(name);
    this.adjList.set(name, []);
  }

  // Undirected: the edge is stored in both directions.
  addPath(from, to, distance, hasStairs = false) {
    this.addLocation(from);
    this.addLocation(to);
    this.adjList.get(from).push({ to, distance, hasStairs });
    this.adjList.get(to).push({ to: from, distance, hasStairs });
  }

  // Wheelchair mode skips any corridor with stairs.
  canUse(edge, avoidStairs) {
    return !(avoidStairs && edge.hasStairs);
  }

  getDistance(from, to) {
    for (const edge of this.adjList.get(from) || []) {
      if (edge.to === to) return edge.distance;
    }
    return -1;
  }

  // Rebuild the route from a parent map and add up the distances.
  reconstruct(parent, start, end) {
    const route = [];
    let node = end;
    while (node) {
      route.push(node);
      node = parent.get(node);
    }
    route.reverse();
    if (route[0] !== start) return null;

    let dist = 0;
    for (let i = 0; i + 1 < route.length; i++) {
      dist += this.getDistance(route[i], route[i + 1]);
    }
    return { path: route, distance: dist, stops: route.length - 1 };
  }

  // BFS - fewest stops. Uses a queue, explores level by level.
  bfsRoute(start, end, avoidStairs) {
    if (!this.locations.has(start) || !this.locations.has(end)) return null;
    if (start === end) return { path: [start], distance: 0, stops: 0 };

    const queue = [start];
    const visited = new Set([start]);
    const parent = new Map([[start, ""]]);

    while (queue.length) {
      const current = queue.shift();
      for (const edge of this.adjList.get(current) || []) {
        if (visited.has(edge.to) || !this.canUse(edge, avoidStairs)) continue;
        visited.add(edge.to);
        parent.set(edge.to, current);
        if (edge.to === end) return this.reconstruct(parent, start, end);
        queue.push(edge.to);
      }
    }
    return null;
  }

  // DFS - all paths using backtracking, sorted by distance.
  dfsExplore(start, end, avoidStairs, maxPaths = 20) {
    if (!this.locations.has(start) || !this.locations.has(end)) return [];
    const all = [];
    const visited = new Set([start]);
    const path = [start];

    const walk = (current, dist) => {
      if (all.length >= maxPaths) return;
      if (current === end) {
        all.push({ path: [...path], distance: dist, stops: path.length - 1 });
        return;
      }
      for (const edge of this.adjList.get(current) || []) {
        if (visited.has(edge.to) || !this.canUse(edge, avoidStairs)) continue;
        visited.add(edge.to);
        path.push(edge.to);
        walk(edge.to, dist + edge.distance);
        path.pop();          // backtrack
        visited.delete(edge.to);
      }
    };

    walk(start, 0);
    all.sort((a, b) => a.distance - b.distance);
    return all;
  }

  // Dijkstra - shortest distance using a min-priority queue.
  dijkstraRoute(start, end, avoidStairs) {
    if (!this.locations.has(start) || !this.locations.has(end)) return null;
    if (start === end) return { path: [start], distance: 0, stops: 0 };

    const dist = new Map();
    const parent = new Map([[start, ""]]);
    for (const loc of this.locations) dist.set(loc, Infinity);
    dist.set(start, 0);

    // simple min-heap using an array
    const pq = [{ d: 0, u: start }];
    const popMin = () => {
      let best = 0;
      for (let i = 1; i < pq.length; i++) {
        if (pq[i].d < pq[best].d) best = i;
      }
      return pq.splice(best, 1)[0];
    };

    while (pq.length) {
      const { d, u } = popMin();
      if (d > dist.get(u)) continue;
      if (u === end) break;
      for (const edge of this.adjList.get(u) || []) {
        if (!this.canUse(edge, avoidStairs)) continue;
        const nd = dist.get(u) + edge.distance;
        if (nd < dist.get(edge.to)) {
          dist.set(edge.to, nd);
          parent.set(edge.to, u);
          pq.push({ d: nd, u: edge.to });
        }
      }
    }

    if (dist.get(end) === Infinity) return null;
    return this.reconstruct(parent, start, end);
  }

  // BFS + DFS + Dijkstra on the same pair, for the Compare button.
  compareRoutes(start, end, avoidStairs) {
    return {
      bfs: this.bfsRoute(start, end, avoidStairs),
      dijkstra: this.dijkstraRoute(start, end, avoidStairs),
      dfs: this.dfsExplore(start, end, avoidStairs),
    };
  }
}
