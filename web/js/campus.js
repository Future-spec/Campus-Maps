export const WINGS = {
  central: { label: "Central Circulation", color: "#c9a227" },
  A: { label: "A-Wing · IT / Computer Labs", color: "#3d8bfd" },
  B: { label: "B-Wing · Halls & Guest", color: "#2bb673" },
  C: { label: "C-Wing · Classrooms & Labs", color: "#e07a3d" },
};

export const NODES = {
  "Central Junction": { x: 600, y: 318, wing: "central", kind: "hub" },
  "Main Lift": { x: 508, y: 318, wing: "central", kind: "lift" },
  "Lobby Stairs": { x: 692, y: 318, wing: "central", kind: "stairs" },

  "A-Wing Corridor": { x: 600, y: 148, wing: "A", kind: "hub" },
  "IT Department": { x: 120, y: 64, wing: "A", kind: "room" },
  "IT Staff Room": { x: 270, y: 64, wing: "A", kind: "room" },
  "System Lab": { x: 420, y: 64, wing: "A", kind: "lab" },
  "Database Lab": { x: 560, y: 64, wing: "A", kind: "lab" },
  "OS Lab": { x: 700, y: 64, wing: "A", kind: "lab" },
  "Cloud Computing Lab": { x: 850, y: 64, wing: "A", kind: "lab" },
  "Project Lab": { x: 1000, y: 64, wing: "A", kind: "lab" },
  "Software Lab": { x: 160, y: 210, wing: "A", kind: "lab" },
  "Ladies & Handicap Restroom": { x: 340, y: 210, wing: "A", kind: "access" },
  "A-Wing Rest Room": { x: 500, y: 210, wing: "A", kind: "room" },
  "A-Wing Fire Lift": { x: 820, y: 210, wing: "A", kind: "lift" },
  "A-Wing Stairs": { x: 980, y: 210, wing: "A", kind: "stairs" },

  "B-Wing Corridor": { x: 280, y: 460, wing: "B", kind: "hub" },
  "Guest Room": { x: 90, y: 410, wing: "B", kind: "room" },
  "Multipurpose Hall": { x: 90, y: 540, wing: "B", kind: "hall" },
  "Auditorium Hall": { x: 280, y: 620, wing: "B", kind: "hall" },
  "B-Wing Rest Room": { x: 430, y: 410, wing: "B", kind: "room" },
  "B-Wing Stairs": { x: 430, y: 540, wing: "B", kind: "stairs" },
  "B-Wing Fire Lift": { x: 150, y: 620, wing: "B", kind: "lift" },

  "C-Wing Corridor": { x: 920, y: 460, wing: "C", kind: "hub" },
  "210 EXTC Lab-8": { x: 700, y: 410, wing: "C", kind: "lab" },
  "211 EXTC Lab-9": { x: 840, y: 410, wing: "C", kind: "lab" },
  "212 Civil & Comp Lab": { x: 980, y: 410, wing: "C", kind: "lab" },
  "213 Computer Centre": { x: 1120, y: 410, wing: "C", kind: "lab" },
  "214 Environmental Lab": { x: 700, y: 555, wing: "C", kind: "lab" },
  "Seminar Hall III": { x: 880, y: 620, wing: "C", kind: "hall" },
  "Gents Room": { x: 1040, y: 555, wing: "C", kind: "room" },
  "C-Wing Fire Lift": { x: 760, y: 620, wing: "C", kind: "lift" },
  "C-Wing Stairs": { x: 1120, y: 620, wing: "C", kind: "stairs" },
};

export const EDGES = [
  ["Central Junction", "A-Wing Corridor", 40],
  ["Central Junction", "B-Wing Corridor", 35],
  ["Central Junction", "C-Wing Corridor", 35],
  ["Central Junction", "Main Lift", 12, { liftRequired: true }],
  ["Central Junction", "Lobby Stairs", 12, { hasStairs: true }],

  ["A-Wing Corridor", "IT Department", 22],
  ["A-Wing Corridor", "IT Staff Room", 18],
  ["A-Wing Corridor", "System Lab", 20],
  ["A-Wing Corridor", "Database Lab", 24],
  ["A-Wing Corridor", "OS Lab", 28],
  ["A-Wing Corridor", "Cloud Computing Lab", 32],
  ["A-Wing Corridor", "Project Lab", 36],
  ["A-Wing Corridor", "Software Lab", 26, { isNarrow: true }],
  ["A-Wing Corridor", "Ladies & Handicap Restroom", 14],
  ["A-Wing Corridor", "A-Wing Rest Room", 16, { isNarrow: true }],
  ["A-Wing Corridor", "A-Wing Stairs", 18, { hasStairs: true }],
  ["A-Wing Corridor", "A-Wing Fire Lift", 16, { liftRequired: true }],
  ["System Lab", "Database Lab", 12],
  ["Database Lab", "OS Lab", 12],
  ["OS Lab", "Cloud Computing Lab", 12],
  ["Cloud Computing Lab", "Project Lab", 12],

  ["B-Wing Corridor", "Guest Room", 20],
  ["B-Wing Corridor", "Multipurpose Hall", 22],
  ["B-Wing Corridor", "B-Wing Rest Room", 14],
  ["B-Wing Corridor", "B-Wing Stairs", 16, { hasStairs: true }],
  ["B-Wing Corridor", "B-Wing Fire Lift", 18, { liftRequired: true }],
  ["B-Wing Corridor", "Auditorium Hall", 28, { hasStairs: true }],
  ["B-Wing Fire Lift", "Auditorium Hall", 14, { liftRequired: true }],
  ["Guest Room", "Multipurpose Hall", 18],
  ["Multipurpose Hall", "Auditorium Hall", 20, { hasStairs: true }],

  ["C-Wing Corridor", "210 EXTC Lab-8", 18],
  ["C-Wing Corridor", "211 EXTC Lab-9", 18],
  ["C-Wing Corridor", "212 Civil & Comp Lab", 18],
  ["C-Wing Corridor", "213 Computer Centre", 40],
  ["C-Wing Corridor", "214 Environmental Lab", 22],
  ["C-Wing Corridor", "Seminar Hall III", 26],
  ["C-Wing Corridor", "Gents Room", 20],
  ["C-Wing Corridor", "C-Wing Stairs", 18, { hasStairs: true }],
  ["C-Wing Corridor", "C-Wing Fire Lift", 16, { liftRequired: true }],
  ["210 EXTC Lab-8", "211 EXTC Lab-9", 15],
  ["211 EXTC Lab-9", "212 Civil & Comp Lab", 15],
  ["212 Civil & Comp Lab", "213 Computer Centre", 15],
  ["214 Environmental Lab", "Seminar Hall III", 16],
  ["Seminar Hall III", "C-Wing Fire Lift", 14, { liftRequired: true }],
];

export const PROFILES = {
  wheelchair: {
    label: "Wheelchair",
    avoidStairs: true,
    avoidNarrow: true,
    avoidLiftRequired: false,
    note: "Skips stairs and narrow passages. Lifts are allowed.",
  },
  visual: {
    label: "Visually impaired",
    avoidStairs: false,
    avoidNarrow: true,
    avoidLiftRequired: false,
    note: "Skips narrow passages only.",
  },
  none: {
    label: "No constraints",
    avoidStairs: false,
    avoidNarrow: false,
    avoidLiftRequired: false,
    note: "Only blocked corridors are skipped.",
  },
};

export function loadCampus(graph) {
  for (const [from, to, distance, flags = {}] of EDGES) {
    graph.addPath(from, to, distance, flags);
  }
}

export function edgeKey(a, b) {
  return [a, b].sort().join("|");
}
