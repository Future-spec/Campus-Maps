// Shared campus data for the browser demo.

import { CampusGraph } from "./graph.js";

export const WINGS = {
  A: { label: "A-WING", color: "#297b70" },
  B: { label: "B-WING", color: "#d27a45" },
  C: { label: "C-WING", color: "#7566a8" },
  central: { label: "CENTRAL", color: "#b8860b" },
};

export const NODES = {
  "Central Junction": { x: 600, y: 320, kind: "hub", wing: "central" },
  "Main Entrance": { x: 120, y: 140, kind: "access", wing: "A" },
  "Library": { x: 360, y: 140, kind: "access", wing: "A" },
  "Lift Lobby": { x: 600, y: 140, kind: "lift", wing: "A" },
  "Stairwell": { x: 840, y: 140, kind: "stairs", wing: "A" },
  "Lab 101": { x: 250, y: 540, kind: "access", wing: "B" },
  "213 Computer Centre": { x: 600, y: 540, kind: "access", wing: "B" },
  "Seminar Hall": { x: 950, y: 540, kind: "access", wing: "C" },
};

// [from, to, distance in metres, accessibility flags]
export const EDGES = [
  ["Main Entrance", "Library", 18, {}],
  ["Library", "Lift Lobby", 22, { liftRequired: true }],
  ["Lift Lobby", "Central Junction", 14, { liftRequired: true }],
  ["Lift Lobby", "Stairwell", 16, { hasStairs: true }],
  ["Stairwell", "Central Junction", 15, { hasStairs: true }],
  ["Central Junction", "Lab 101", 20, {}],
  ["Central Junction", "213 Computer Centre", 24, {}],
  ["Central Junction", "Seminar Hall", 30, { isNarrow: true }],
  ["Lab 101", "213 Computer Centre", 16, { isNarrow: true }],
  ["213 Computer Centre", "Seminar Hall", 18, {}],
];

export const PROFILES = {
  wheelchair: {
    label: "Wheelchair",
    note: "Avoids stairs and narrow passages; lift routes remain available.",
    avoidStairs: true,
    avoidNarrow: true,
    avoidLiftRequired: false,
  },
  visual: {
    label: "Visually impaired",
    note: "Avoids narrow passages and lift-only connections.",
    avoidStairs: false,
    avoidNarrow: true,
    avoidLiftRequired: true,
  },
  none: {
    label: "No constraints",
    note: "Shows every available corridor unless it has been blocked.",
    avoidStairs: false,
    avoidNarrow: false,
    avoidLiftRequired: false,
  },
};

export function edgeKey(from, to) {
  return [from, to].sort().join("::");
}

export function loadCampus(graph) {
  Object.keys(NODES).forEach((name) => graph.addLocation(name));
  EDGES.forEach(([from, to, distance, flags]) => graph.addPath(from, to, distance, flags));
  return graph;
}

export function createCampusGraph() {
  return loadCampus(new CampusGraph());
}
