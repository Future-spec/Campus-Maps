import { CampusGraph } from "./graph.js";

export const LOCATIONS = [
  "Entrance", "Main Corridor", "Lab", "Library",
  "Lift", "Stairs", "Seminar Hall",
];

// [from, to, distance in metres, hasStairs]
export const EDGES = [
  [0, 1, 10, false], [1, 6, 50, false],
  [1, 2, 10, false], [2, 3, 10, false], [3, 6, 10, false],
  [1, 4, 5, false], [4, 6, 20, false],
  [1, 5, 5, true], [5, 6, 10, true],
];

export const POSITIONS = [
  [85, 180], [260, 180], [430, 70], [630, 70],
  [440, 315], [635, 315], [850, 180],
];

export function createCampusGraph() {
  const graph = new CampusGraph(LOCATIONS);
  EDGES.forEach(([from, to, distance, hasStairs]) => {
    graph.addPath(from, to, distance, hasStairs);
  });
  return graph;
}
