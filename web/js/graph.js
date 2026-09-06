// Browser version of the same small adjacency-list graph used in C++.

export class CampusGraph {
  constructor(locations) {
    this.locations = locations;
    this.adjList = locations.map(() => []);
  }

  addPath(from, to, distance, hasStairs = false) {
    this.adjList[from].push({ to, distance, hasStairs });
    this.adjList[to].push({ to: from, distance, hasStairs });
  }

  canUse(edge, wheelchairMode) {
    return !(wheelchairMode && edge.hasStairs);
  }

  routeDistance(route) {
    return route.slice(0, -1).reduce((total, from, index) => {
      const edge = this.adjList[from].find((item) => item.to === route[index + 1]);
      return total + edge.distance;
    }, 0);
  }

  makeRoute(parent, start, end) {
    const route = [];
    for (let current = end; current !== -1; current = parent[current]) route.push(current);
    route.reverse();
    return route[0] === start ? route : [];
  }

  // BFS: queue-based, finds the fewest-stop route.
  bfsRoute(start, end, wheelchairMode) {
    const queue = [start];
    const visited = Array(this.locations.length).fill(false);
    const parent = Array(this.locations.length).fill(-1);
    visited[start] = true;

    for (let front = 0; front < queue.length; front += 1) {
      const current = queue[front];
      if (current === end) break;
      for (const edge of this.adjList[current]) {
        if (visited[edge.to] || !this.canUse(edge, wheelchairMode)) continue;
        visited[edge.to] = true;
        parent[edge.to] = current;
        queue.push(edge.to);
      }
    }
    return visited[end] ? this.makeRoute(parent, start, end) : [];
  }

  // Dijkstra: repeatedly chooses the smallest total distance.
  dijkstraRoute(start, end, wheelchairMode) {
    const distance = Array(this.locations.length).fill(Infinity);
    const parent = Array(this.locations.length).fill(-1);
    const pending = [{ node: start, distance: 0 }];
    distance[start] = 0;

    while (pending.length) {
      pending.sort((a, b) => a.distance - b.distance);
      const current = pending.shift();
      if (current.distance !== distance[current.node]) continue;
      if (current.node === end) break;

      for (const edge of this.adjList[current.node]) {
        if (!this.canUse(edge, wheelchairMode)) continue;
        const newDistance = current.distance + edge.distance;
        if (newDistance < distance[edge.to]) {
          distance[edge.to] = newDistance;
          parent[edge.to] = current.node;
          pending.push({ node: edge.to, distance: newDistance });
        }
      }
    }
    return Number.isFinite(distance[end]) ? this.makeRoute(parent, start, end) : [];
  }

  // DFS: goes deep first, then returns to try another neighbour.
  dfsTraversal(start, wheelchairMode) {
    const visited = Array(this.locations.length).fill(false);
    const order = [];
    const visit = (current) => {
      visited[current] = true;
      order.push(current);
      for (const edge of this.adjList[current]) {
        if (!visited[edge.to] && this.canUse(edge, wheelchairMode)) visit(edge.to);
      }
    };
    visit(start);
    return order;
  }
}
