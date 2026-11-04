# Patrolling staff can cross bridges

Staff walking their rounds can cross a footbridge, like the bridge over the stream in the Lost Kingdom. In the
original they only crossed one when sent to a job on the other side. A bridge outside a worker's patrol area is still
never taken.

| | |
| --- | --- |
| Id | staff-cross-bridges |
| Version | 1.0.0 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Unchanged |
| Hooks | staff_bridge_wander |
| Needs | STP/TPW Launcher 0.3.0 or later |

## What it changes

Staff with nothing to do walk their rounds: a few steps along the paths in a random direction, then a few more. Players
noticed that cleaners (janitors in the US and Gold editions) crossed a bridge only on the way to a job, such as litter
or a full bin on the far bank, and never while just walking about. That is how the original works: a walk with no job
steps only from one path to the next, and a bridge is not path, so the walk stops at its end and turns back.

With the mod on, a worker's round can also step onto a bridge from the path at either end, walk along it, and step off
onto the path on the other side. It works for every job: cleaners, mechanics, entertainers, guards and researchers
(scientists in the European edition). Everything else about the round is the original's:

- **Patrol areas.** A worker with a patrol area keeps to it. If the bridge, or the far bank, is outside the area, the
  worker never takes it, and if only part of the bridge is inside, the worker walks to the edge of the area and turns
  back. Give a worker an area that covers both ends of the bridge to have them cross.
- **The walk itself.** The same number of steps, never straight back, never into a queue or out of the exit, and the
  same random spot to stand at the end. A round may end on the bridge, where the worker stands for a moment before
  walking on, as on any path.
- **Visitors** walk exactly as before. They cross bridges on the way to a ride or a shop, as in the original.

A bridge needs path at its ends for this: a worker steps onto it from path and off it onto path. In the Lost Kingdom
the bridge over the stream starts outside the park fence, with grass at both ends, so first buy the land around it
(the park fence moves out over the bridge), then lay a path up to each end.

In a park with no bridge, or a park whose bridges are not joined to a path, nothing changes: every round, and every
random number the game draws for it, is the same as with the mod off. Of the four parks only the Lost Kingdom has a
footbridge. The pavement outside the park gate in every park is the same kind of ground as a bridge to the game, and
the mod leaves it out, so staff do not wander out of the gate.

## Details for modders

| What | Function | Mac 2.0.1 |
|---|---|---|
| The random walk, and the hook (Peep.cpp, TPW_MOD_HOOK staff_bridge_wander) | CPeep::SetRandomDestination | 0x10113870 |
| A walk to a target, which lets walkable cells through | MAP_wall_in_way | 0x10101ca4 |
| Where bridge cells get their mode (0x1e from attribute 0x80; flag 0x80 from attribute 0x10) | CMapCell::Reset | 0x1009cb48 |
| Bridge and pavement cells: mode 0x1e | CMapCell::IsWalkable | 0x1009c6ac |
| Path links, between path cells only | CMapCell::CalculateNeighbours | 0x10099998 |

- **A bridge cell** is a map cell of mode 0x1e (CMapCell::IsWalkable) without flag 0x80. CMapCell::Reset gives mode 0x1e
  to the cells whose level attribute has bit 0x80 and no height field status: the Lost Kingdom's footbridge (four cells,
  attribute 0x80) and the pavement outside the gate of every park (attributes 0x90 and 0x94). The pavement's bit 0x10
  gives it flag 0x80, which the bridge lacks.
- **Path links:** the random walk steps along a cell's neighbour bits 0x10 (+Y), 0x04 (+X), 0x01 (-Y) and 0x40 (-X).
  CMapCell::CalculateNeighbours sets them between path cells only, so no path cell has a link toward a bridge, and a
  bridge cell has none at all.
- **The hook:** with gModStaffBridgeWander set and a staff member walking (peep types 4 to 8), each step also offers the
  neighbours a link does not: a bridge cell beside a path or bridge cell, and a path cell beside a bridge cell. They join
  the path links before the original's patrol area, queue and exit tests, so those tests drop them as they drop any
  other step. Where no such neighbour exists, the step and its random numbers are the original's.

## Saved games

Saved games are unchanged: a park saved with the mod on loads and plays with it off. A worker saved standing on a
bridge, loaded with the mod off, walks back to the nearest path, as the original does with a worker it finds off the
paths.

## Tests

tests/test_staff_cross_bridges.cpp runs the port's CPeep::SetRandomDestination with the mod off and on in a small park
drawn into a real map grid: path on both banks of a stream, a four cell footbridge across it, and a row of gate
pavement beside a path. The functions it calls answer from the cells as the game's do; the random numbers come from a
fixed generator.

- Mod off: thousands of rounds reach the end of the bridge and never step onto it (tpw-decomp's differential test
  tests/test_CPeep_SetRandomDestination.py compares this code with the Mac's program).
- Mod on: a cleaner steps onto the bridge, walks across, steps off onto the far bank and comes back, every step onto a
  path or bridge cell beside the last; every staff job crosses; a worker standing on the bridge walks along or off it.
- Patrol areas: a bridge outside the area is never taken, and a bridge half inside it is walked to the edge and no
  further, from either bank.
- Visitors: the same rounds and random numbers with the mod on and off.
- No bridge: for every staff job, from several places, with and without a patrol area, the same rounds and random
  numbers with the mod on and off.

They run with the port's tests (`ctest -R mod_staff_cross_bridges`).
