# PlayStation rides

Adds the rides only the PlayStation edition had, converted from your own PlayStation disc. It stays locked until that
disc's rides are in the data folder. A park that uses them needs them to load.

| | |
| --- | --- |
| Id | ps1-rides |
| Version | 0.1.0 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Changed: a park that uses one of its rides needs the mod to load |
| Hooks | None (content: assets/ps1) |
| Needs | STP/TPW Launcher 0.3.3 or later, and your own PlayStation disc |

## What it changes

The PlayStation edition had rides the PC and Mac editions never did. This mod adds them to the game, each in its
theme's ride list, built from your own PlayStation disc. The repository holds no game files: the rides are made on your
machine from your disc, into the data folder's assets/ps1, and the Mods box shows the mod locked until they are there.

Each ride keeps the number the game saves it by (201 and up, apart from the Gold Edition's), so a park that uses one
says so in its file:

- **Loading.** A park with a PlayStation ride loads only with the mod on and that ride present. Without it the game says
  which rides are missing and goes back, and the park file is left as it was, so you can add the rides and carry on.
- **Online.** A park with PlayStation rides can be published. Its page says which rides it needs, and a player without
  them is told so and cannot visit or download it.

With the mod off the game is the original: the rides are not in the lists, and parks that use none of them load and
save exactly as before.

## Not yet the original's

The rides' scripts, animations, sounds and run times are made up where the conversion has nothing to go by. The goal is
the PlayStation edition's own behaviour; they will be brought closer to it.
