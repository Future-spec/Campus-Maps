import { LOCATIONS, EDGES, POSITIONS, createCampusGraph } from "./campus.js";

const graph = createCampusGraph();
const state = { start: 0, end: 6, wheelchairMode: false, route: [] };
const $ = (id) => document.getElementById(id);

function fillLocations(select) {
  select.innerHTML = LOCATIONS.map((name, index) => (
    `<option value="${index}">${index}. ${name}</option>`
  )).join("");
}

function edgeKey(from, to) {
  return [from, to].sort((a, b) => a - b).join("-");
}

function drawMap() {
  const highlighted = new Set();
  for (let i = 0; i + 1 < state.route.length; i += 1) {
    highlighted.add(edgeKey(state.route[i], state.route[i + 1]));
  }

  const edges = EDGES.map(([from, to, distance, hasStairs]) => {
    const [x1, y1] = POSITIONS[from];
    const [x2, y2] = POSITIONS[to];
    const classes = `edge${hasStairs ? " stairs" : ""}${highlighted.has(edgeKey(from, to)) ? " route" : ""}`;
    return `<g><line class="${classes}" x1="${x1}" y1="${y1}" x2="${x2}" y2="${y2}"/>
      <text class="distance" x="${(x1 + x2) / 2}" y="${(y1 + y2) / 2 - 7}">${distance}m</text></g>`;
  }).join("");

  const nodes = LOCATIONS.map((name, index) => {
    const [x, y] = POSITIONS[index];
    const classes = `node${index === state.start ? " start" : ""}${index === state.end ? " end" : ""}`;
    return `<g class="${classes}"><circle cx="${x}" cy="${y}" r="24"/>
      <text x="${x}" y="${y + 5}">${index}</text><text class="node-name" x="${x}" y="${y + 44}">${name}</text></g>`;
  }).join("");

  $("map").innerHTML = edges + nodes;
}

function routeText(route) {
  return route.map((location) => LOCATIONS[location]).join(" -> ");
}

function showRoute(algorithm) {
  const route = algorithm === "bfs"
    ? graph.bfsRoute(state.start, state.end, state.wheelchairMode)
    : graph.dijkstraRoute(state.start, state.end, state.wheelchairMode);
  state.route = route;
  drawMap();

  if (!route.length) {
    $("result").innerHTML = "<strong>No route found.</strong> Try turning wheelchair mode off.";
    return;
  }
  const title = algorithm === "bfs" ? "BFS: fewest stops" : "Dijkstra: shortest distance";
  $("result").innerHTML = `<strong>${title}</strong><p>${routeText(route)}</p>
    <p>Stops: ${route.length - 1} | Distance: ${graph.routeDistance(route)} m</p>`;
}

function showDfs() {
  const order = graph.dfsTraversal(state.start, state.wheelchairMode);
  state.route = [];
  drawMap();
  $("result").innerHTML = `<strong>DFS traversal from ${LOCATIONS[state.start]}</strong>
    <p>${routeText(order)}</p><p>DFS goes deep first, then backtracks.</p>`;
}

function updateModeNote() {
  $("modeNote").textContent = state.wheelchairMode
    ? "Wheelchair mode is ON: red stair edges are skipped."
    : "Wheelchair mode is OFF: every corridor can be used.";
}

window.addEventListener("DOMContentLoaded", () => {
  fillLocations($("start"));
  fillLocations($("end"));
  $("start").value = state.start;
  $("end").value = state.end;

  $("start").addEventListener("change", (event) => {
    state.start = Number(event.target.value);
    state.route = [];
    drawMap();
  });
  $("end").addEventListener("change", (event) => {
    state.end = Number(event.target.value);
    state.route = [];
    drawMap();
  });
  $("wheelchair").addEventListener("change", (event) => {
    state.wheelchairMode = event.target.checked;
    state.route = [];
    updateModeNote();
    drawMap();
  });
  $("bfs").addEventListener("click", () => showRoute("bfs"));
  $("dijkstra").addEventListener("click", () => showRoute("dijkstra"));
  $("dfs").addEventListener("click", showDfs);

  updateModeNote();
  drawMap();
});
