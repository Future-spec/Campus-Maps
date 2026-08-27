/**
 * Campus Accessibility Route Finder — Graph ADT
 * Same algorithms as graph.cpp: adjacency list, BFS, DFS, Dijkstra.
 */

export class CampusGraph {
  constructor() {
    this.adjList = new Map();
    this.locations = new Set();
  }

  addLocation(name) {
    if (this.locations.has(name)) return false;
    this.locations.add(name);
    this.adjList.set(name, []);
    return true;
  }

  addPath(from, to, distance, flags = {}) {
    const { hasStairs = false, isNarrow = false, liftRequired = false } = flags;
    this.addLocation(from);
    this.addLocation(to);
    if (this.adjList.get(from).some((p) => p.to === to)) return false;

    const edge = { from, to, distance, hasStairs, isNarrow, liftRequired, isBlocked: false };
    const reverse = { ...edge, from: to, to: from };
    this.adjList.get(from).push(edge);
    this.adjList.get(to).push(reverse);
    return true;
  }

  removePath(from, to) {
    const a = this.adjList.get(from);
    const b = this.adjList.get(to);
    if (!a || !b) return false;
    const before = a.length;
    this.adjList.set(from, a.filter((p) => p.to !== to));
    this.adjList.set(to, b.filter((p) => p.to !== from));
    return this.adjList.get(from).length !== before;
  }

  setBlocked(from, to, blocked) {
    let found = false;
    for (const p of this.adjList.get(from) || []) {
      if (p.to === to) {
        p.isBlocked = blocked;
        found = true;
      }
    }
    for (const p of this.adjList.get(to) || []) {
      if (p.to === from) p.isBlocked = blocked;
    }
    return found;
  }

  markAccessibility(from, to, flags) {
    const apply = (list, dest) => {
      for (const p of list || []) {
        if (p.to === dest) {
          p.hasStairs = !!flags.hasStairs;
          p.isNarrow = !!flags.isNarrow;
          p.liftRequired = !!flags.liftRequired;
          return true;
        }
      }
      return false;
    };
    const found = apply(this.adjList.get(from), to);
    apply(this.adjList.get(to), from);
    return found;
  }

  isAccessible(path, constraints) {
    if (path.isBlocked) return false;
    if (constraints.avoidStairs && path.hasStairs) return false;
    if (constraints.avoidNarrow && path.isNarrow) return false;
    if (constraints.avoidLiftRequired && path.liftRequired) return false;
    return true;
  }

  getEdge(from, to) {
    return (this.adjList.get(from) || []).find((p) => p.to === to) || null;
  }

  getEdgeDistance(from, to) {
    const e = this.getEdge(from, to);
    return e ? e.distance : -1;
  }

  reconstruct(parent, start, end) {
    const route = [];
    let node = end;
    while (node) {
      route.push(node);
      node = parent.get(node);
    }
    route.reverse();
    if (route[0] !== start) return [];
    let dist = 0;
    for (let i = 0; i + 1 < route.length; i++) {
      dist += this.getEdgeDistance(route[i], route[i + 1]);
    }
    return { path: route, distance: dist, stops: route.length - 1 };
  }

  /** BFS: fewest hops. O(V + E) */
  bfsRoute(start, end, constraints) {
    if (!this.locations.has(start) || !this.locations.has(end)) return null;
    if (start === end) return { path: [start], distance: 0, stops: 0 };

    const q = [start];
    const visited = new Set([start]);
    const parent = new Map([[start, ""]]);

    while (q.length) {
      const current = q.shift();
      for (const path of this.adjList.get(current) || []) {
        if (visited.has(path.to) || !this.isAccessible(path, constraints)) continue;
        visited.add(path.to);
        parent.set(path.to, current);
        if (path.to === end) return this.reconstruct(parent, start, end);
        q.push(path.to);
      }
    }
    return null;
  }

  /** DFS + backtracking: all paths, capped. */
  dfsExplore(start, end, constraints, maxPaths = 20) {
    if (!this.locations.has(start) || !this.locations.has(end)) return [];
    const all = [];
    const visited = new Set([start]);
    const currentPath = [start];

    const walk = (current, dist) => {
      if (all.length >= maxPaths) return;
      if (current === end) {
        all.push({ path: [...currentPath], distance: dist, stops: currentPath.length - 1 });
        return;
      }
      for (const edge of this.adjList.get(current) || []) {
        if (visited.has(edge.to) || !this.isAccessible(edge, constraints)) continue;
        visited.add(edge.to);
        currentPath.push(edge.to);
        walk(edge.to, dist + edge.distance);
        currentPath.pop();
        visited.delete(edge.to);
      }
    };

    walk(start, 0);
    all.sort((a, b) => a.distance - b.distance);
    return all;
  }

  /** Dijkstra: shortest distance. O((V+E) log V) */
  dijkstraRoute(start, end, constraints) {
    if (!this.locations.has(start) || !this.locations.has(end)) return null;
    if (start === end) return { path: [start], distance: 0, stops: 0 };

    const dist = new Map();
    const parent = new Map([[start, ""]]);
    for (const loc of this.locations) dist.set(loc, Infinity);
    dist.set(start, 0);

    const pq = [{ d: 0, u: start }];
    const popMin = () => {
      let best = 0;
      for (let i = 1; i < pq.length; i++) if (pq[i].d < pq[best].d) best = i;
      return pq.splice(best, 1)[0];
    };

    while (pq.length) {
      const { d, u } = popMin();
      if (d > dist.get(u)) continue;
      if (u === end) break;
      for (const edge of this.adjList.get(u) || []) {
        if (!this.isAccessible(edge, constraints)) continue;
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

  showInaccessible(start, constraints) {
    if (!this.locations.has(start)) return [];
    const reachable = new Set([start]);
    const q = [start];
    while (q.length) {
      const current = q.shift();
      for (const path of this.adjList.get(current) || []) {
        if (reachable.has(path.to) || !this.isAccessible(path, constraints)) continue;
        reachable.add(path.to);
        q.push(path.to);
      }
    }
    return [...this.locations].filter((loc) => !reachable.has(loc)).sort();
  }

  compareRoutes(start, end, constraints) {
    const bfs = this.bfsRoute(start, end, constraints);
    const dijkstra = this.dijkstraRoute(start, end, constraints);
    const dfs = this.dfsExplore(start, end, constraints);
    return { bfs, dijkstra, dfs };
  }

  snapshot() {
    const nodes = [...this.locations].sort();
    const edges = [];
    const seen = new Set();
    for (const from of nodes) {
      for (const p of this.adjList.get(from) || []) {
        const key = [from, p.to].sort().join("|");
        if (seen.has(key)) continue;
        seen.add(key);
        edges.push({ ...p });
      }
    }
    return { nodes, edges };
  }
}
