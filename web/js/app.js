import { CampusGraph } from "./graph.js";
import { NODES, WINGS, EDGES, PROFILES, loadCampus, edgeKey } from "./campus.js";

const graph = new CampusGraph();
loadCampus(graph);

const state = {
  start: "Central Junction",
  end: "213 Computer Centre",
  profile: "wheelchair",
  picking: "start",
  highlight: [],
  inaccessible: [],
};

const $ = (id) => document.getElementById(id);

function constraints() {
  const p = PROFILES[state.profile];
  return {
    avoidStairs: p.avoidStairs,
    avoidNarrow: p.avoidNarrow,
    avoidLiftRequired: p.avoidLiftRequired,
  };
}

function fillSelects() {
  const names = [...graph.locations].sort();
  for (const id of ["start", "end"]) {
    const el = $(id);
    el.innerHTML = names.map((n) => `<option value="${n}">${n}</option>`).join("");
  }
  $("start").value = state.start;
  $("end").value = state.end;
}

function kindColor(kind, wing) {
  if (kind === "stairs") return "#c0392b";
  if (kind === "lift") return "#1e6f5c";
  if (kind === "access") return "#2bb673";
  if (kind === "hub") return "#b8860b";
  return WINGS[wing].color;
}

function drawMap() {
  const svg = $("floor");
  const routeSet = new Set();
  for (let i = 0; i + 1 < state.highlight.length; i++) {
    routeSet.add(edgeKey(state.highlight[i], state.highlight[i + 1]));
  }
  const blockedLook = new Set(state.inaccessible);

  const wingRects = `
    <rect class="wing-box wing-a" x="40" y="24" width="1120" height="220" rx="18"/>
    <text class="wing-label" x="56" y="46">A-WING</text>
    <rect class="wing-box wing-mid" x="430" y="268" width="340" height="100" rx="16"/>
    <text class="wing-label" x="446" y="290">CENTRAL</text>
    <rect class="wing-box wing-b" x="40" y="380" width="500" height="280" rx="18"/>
    <text class="wing-label" x="56" y="402">B-WING</text>
    <rect class="wing-box wing-c" x="640" y="380" width="520" height="280" rx="18"/>
    <text class="wing-label" x="656" y="402">C-WING</text>
  `;

  const edgeLines = EDGES.map(([from, to, , flags = {}]) => {
    const a = NODES[from];
    const b = NODES[to];
    const e = graph.getEdge(from, to);
    const cls = [
      "edge",
      flags.hasStairs || e?.hasStairs ? "stairs" : "",
      flags.isNarrow || e?.isNarrow ? "narrow" : "",
      flags.liftRequired || e?.liftRequired ? "lift" : "",
      e?.isBlocked ? "blocked" : "",
      routeSet.has(edgeKey(from, to)) ? "route" : "",
    ].join(" ");
    return `<line class="${cls}" data-from="${from}" data-to="${to}" x1="${a.x}" y1="${a.y}" x2="${b.x}" y2="${b.y}"/>`;
  }).join("");

  const nodes = Object.entries(NODES).map(([name, n]) => {
    const cls = [
      "node",
      name === state.start ? "start" : "",
      name === state.end ? "end" : "",
      blockedLook.has(name) ? "inaccessible" : "",
    ].join(" ");
    const r = n.kind === "hub" ? 11 : 8;
    const label = name.length > 22 ? name.slice(0, 20) + "…" : name;
    return `<g class="${cls}" data-name="${name}">
      <circle cx="${n.x}" cy="${n.y}" r="${r}" fill="${kindColor(n.kind, n.wing)}"/>
      <text x="${n.x + 12}" y="${n.y + 4}">${label}</text>
    </g>`;
  }).join("");

  svg.innerHTML = wingRects + edgeLines + nodes;

  // click nodes to pick start/end
  svg.querySelectorAll(".node").forEach((g) => {
    g.addEventListener("click", () => {
      const name = g.dataset.name;
      if (state.picking !== "end") {
        state.start = name;
        $("start").value = name;
        state.picking = "end";
        showStatus(`Start set to ${name}. Click a destination room.`);
      } else {
        state.end = name;
        $("end").value = name;
        state.picking = "start";
        showStatus(`Route set: ${state.start} → ${state.end}. Run BFS, Dijkstra, or Compare.`);
      }
      drawMap();
    });
  });

  // click edges to block/unblock
  svg.querySelectorAll(".edge").forEach((line) => {
    line.addEventListener("click", (ev) => {
      ev.stopPropagation();
      const from = line.dataset.from;
      const to = line.dataset.to;
      const e = graph.getEdge(from, to);
      graph.setBlocked(from, to, !e.isBlocked);
      showStatus(`${e.isBlocked ? "Blocked" : "Unblocked"}: ${from} ↔ ${to}`);
      drawMap();
    });
  });
}

function formatPath(result) {
  if (!result) return "No accessible route.";
  return `${result.path.join(" → ")}\n${result.distance} m · ${result.stops} stop(s)`;
}

function showStatus(text) {
  $("status").textContent = text;
}

function runBfs() {
  const r = graph.bfsRoute(state.start, state.end, constraints());
  state.highlight = r ? r.path : [];
  state.inaccessible = [];
  $("output").innerHTML = `<div class="compare-item"><h3>BFS - Fewest Stops</h3><div class="path-line">${formatPath(r)}</div></div>`;
  drawMap();
}

function runDijkstra() {
  const r = graph.dijkstraRoute(state.start, state.end, constraints());
  state.highlight = r ? r.path : [];
  state.inaccessible = [];
  $("output").innerHTML = `<div class="compare-item"><h3>Dijkstra - Shortest Distance</h3><div class="path-line">${formatPath(r)}</div></div>`;
  drawMap();
}

function runDfs() {
  const all = graph.dfsExplore(state.start, state.end, constraints());
  state.highlight = all[0] ? all[0].path : [];
  state.inaccessible = [];
  const items = all.length
    ? all.map((p, i) => `<li>${p.path.join(" → ")} <em>(${p.distance}m)</em></li>`).join("")
    : "<li>No accessible paths.</li>";
  $("output").innerHTML = `<div class="compare-item"><h3>DFS - All Paths</h3><ol>${items}</ol></div>`;
  drawMap();
}

function runCompare() {
  const { bfs, dijkstra, dfs } = graph.compareRoutes(state.start, state.end, constraints());
  state.highlight = dijkstra ? dijkstra.path : bfs ? bfs.path : [];
  state.inaccessible = [];
  let summary = "Same route from both algorithms.";
  if (bfs && dijkstra && bfs.distance !== dijkstra.distance) {
    summary = `BFS minimises stops (${bfs.stops}). Dijkstra minimises distance (${dijkstra.distance}m vs ${bfs.distance}m).`;
  }
  $("output").innerHTML = `
    <div class="compare-grid">
      <div class="compare-item"><h3>BFS</h3><div class="path-line">${formatPath(bfs)}</div></div>
      <div class="compare-item"><h3>Dijkstra</h3><div class="path-line">${formatPath(dijkstra)}</div></div>
      <div class="compare-item"><h3>DFS found ${dfs.length} path(s)</h3><p>${summary}</p></div>
    </div>`;
  drawMap();
}

function runInaccessible() {
  const list = graph.showInaccessible(state.start, constraints());
  state.inaccessible = list;
  state.highlight = [];
  $("output").innerHTML = `<div class="compare-item"><h3>Unreachable from ${state.start}</h3>${
    list.length ? `<ol>${list.map((n) => `<li>${n}</li>`).join("")}</ol>` : "<p>Every location is reachable.</p>"
  }</div>`;
  drawMap();
}

window.addEventListener("DOMContentLoaded", () => {
  fillSelects();
  $("profile").value = state.profile;
  $("profileNote").textContent = PROFILES[state.profile].note;
  drawMap();

  $("start").addEventListener("change", (e) => {
    state.start = e.target.value;
    drawMap();
  });
  $("end").addEventListener("change", (e) => {
    state.end = e.target.value;
    drawMap();
  });
  $("profile").addEventListener("change", (e) => {
    state.profile = e.target.value;
    $("profileNote").textContent = PROFILES[state.profile].note;
  });
  $("btnBfs").addEventListener("click", runBfs);
  $("btnDijkstra").addEventListener("click", runDijkstra);
  $("btnDfs").addEventListener("click", runDfs);
  $("btnCompare").addEventListener("click", runCompare);
  $("btnUnreachable").addEventListener("click", runInaccessible);
  $("btnReset").addEventListener("click", () => {
    location.reload();
  });
});
