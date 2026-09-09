// Accessibility-aware weighted graph used by the browser route finder.

export class CampusGraph {
  constructor() {
    this.adjList = new Map();
  }

  get locations() {
    return new Set(this.adjList.keys());
  }

  addLocation(name) {
    if (!this.adjList.has(name)) this.adjList.set(name, []);
  }

  addPath(from, to, distance, flags = {}) {
    this.addLocation(from);
    this.addLocation(to);
    this.adjList.get(from).push({ to, distance, ...flags, isBlocked: false });
    this.adjList.get(to).push({ to: from, distance, ...flags, isBlocked: false });
  }

  getEdge(from, to) {
    return (this.adjList.get(from) || []).find((edge) => edge.to === to);
  }

  setBlocked(from, to, blocked) {
    const forward = this.getEdge(from, to);
    const reverse = this.getEdge(to, from);
    if (forward) forward.isBlocked = blocked;
    if (reverse) reverse.isBlocked = blocked;
  }

  canUse(edge, options) {
    return !edge.isBlocked &&
      !(options.avoidStairs && edge.hasStairs) &&
      !(options.avoidNarrow && edge.isNarrow) &&
      !(options.avoidLiftRequired && edge.liftRequired);
  }

  reconstruct(parent, start, end) {
    const path = [];
    let current = end;
    while (current !== undefined) {
      path.push(current);
      current = parent.get(current);
    }
    path.reverse();
    if (path[0] !== start) return null;
    let distance = 0;
    for (let index = 0; index < path.length - 1; index += 1) {
      distance += this.getEdge(path[index], path[index + 1]).distance;
    }
    return { path, distance, stops: path.length - 1 };
  }

  bfsRoute(start, end, options) {
    if (!this.adjList.has(start) || !this.adjList.has(end)) return null;
    const queue = [start];
    const visited = new Set([start]);
    const parent = new Map();
    while (queue.length) {
      const current = queue.shift();
      if (current === end) return this.reconstruct(parent, start, end);
      for (const edge of this.adjList.get(current)) {
        if (visited.has(edge.to) || !this.canUse(edge, options)) continue;
        visited.add(edge.to);
        parent.set(edge.to, current);
        queue.push(edge.to);
      }
    }
    return null;
  }

  dijkstraRoute(start, end, options) {
    const distances = new Map([...this.adjList.keys()].map((name) => [name, Infinity]));
    const parent = new Map();
    distances.set(start, 0);
    const queue = [{ name: start, distance: 0 }];
    while (queue.length) {
      queue.sort((a, b) => a.distance - b.distance);
      const current = queue.shift();
      if (current.name === end) return this.reconstruct(parent, start, end);
      if (current.distance > distances.get(current.name)) continue;
      for (const edge of this.adjList.get(current.name) || []) {
        if (!this.canUse(edge, options)) continue;
        const next = current.distance + edge.distance;
        if (next < distances.get(edge.to)) {
          distances.set(edge.to, next);
          parent.set(edge.to, current.name);
          queue.push({ name: edge.to, distance: next });
        }
      }
    }
    return null;
  }

  dfsExplore(start, end, options, maxPaths = 20) {
    const routes = [];
    const visited = new Set([start]);
    const path = [start];
    const walk = (current, distance) => {
      if (routes.length >= maxPaths) return;
      if (current === end) {
        routes.push({ path: [...path], distance, stops: path.length - 1 });
        return;
      }
      for (const edge of this.adjList.get(current) || []) {
        if (visited.has(edge.to) || !this.canUse(edge, options)) continue;
        visited.add(edge.to);
        path.push(edge.to);
        walk(edge.to, distance + edge.distance);
        path.pop();
        visited.delete(edge.to);
      }
    };
    walk(start, 0);
    return routes.sort((a, b) => a.distance - b.distance);
  }

  compareRoutes(start, end, options) {
    return {
      bfs: this.bfsRoute(start, end, options),
      dijkstra: this.dijkstraRoute(start, end, options),
      dfs: this.dfsExplore(start, end, options),
    };
  }

  showInaccessible(start, options) {
    const reachable = new Set();
    const queue = [start];
    while (queue.length) {
      const current = queue.shift();
      if (reachable.has(current)) continue;
      reachable.add(current);
      for (const edge of this.adjList.get(current) || []) {
        if (this.canUse(edge, options)) queue.push(edge.to);
      }
    }
    return [...this.adjList.keys()].filter((name) => !reachable.has(name));
  }
}
