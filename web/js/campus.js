// campus.js - small campus graph data for the web demo.
// Mirrors the C++ loadCampus() in main.cpp: same 7 locations,
// same 9 corridors, and the same two stair corridors.

import { CampusGraph } from "./graph.js";

export const LOCATIONS = [
  "Entrance",
  "Main Corridor",
  "Lab",
  "Library",
  "Lift",
  "Stairs",
  "Seminar Hall",
];

// [fromIndex, toIndex, distance in metres, hasStairs]
export const EDGES = [
  [0, 1, 10, false],   // Entrance      - Main Corridor
  [1, 6, 50, false],   // Main Corridor - Seminar Hall
  [1, 2, 10, false],   // Main Corridor - Lab
  [2, 3, 10, false],   // Lab           - Library
  [3, 6, 10, false],   // Library       - Seminar Hall
  [1, 4,  5, false],   // Main Corridor - Lift
  [4, 6, 20, false],   // Lift          - Seminar Hall
  [1, 5,  5, true],    // Main Corridor - Stairs   (stairs)
  [5, 6, 10, true],    // Stairs        - Seminar Hall (stairs)
];

// x, y coordinates for each location on the SVG map
export const POSITIONS = {
  "Entrance":      [ 85, 180],
  "Main Corridor": [260, 180],
  "Lab":           [430,  70],
  "Library":       [630,  70],
  "Lift":          [440, 315],
  "Stairs":        [635, 315],
  "Seminar Hall":  [850, 180],
};

export function createCampusGraph() {
  const graph = new CampusGraph();
  EDGES.forEach(([from, to, distance, hasStairs]) => {
    graph.addPath(LOCATIONS[from], LOCATIONS[to], distance, hasStairs);
  });
  return graph;
}
