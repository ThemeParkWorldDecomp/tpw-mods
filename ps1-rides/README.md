# PlayStation rides

Adds the seven rides only the PlayStation edition had, and its Tower as a Halloween scenery object, converted from your
own PlayStation disc. It stays locked until that disc's rides are in the data folder. A park that uses them needs them
to load.

| | |
| --- | --- |
| Id | ps1-rides |
| Version | 0.1.1 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Changed: a park that uses one of its rides or the Tower needs the mod to load |
| Hooks | ps1_badge (content: assets/ps1) |
| Needs | STP/TPW Launcher 0.4.0 or later, and your own PlayStation disc |

## What it changes

The PlayStation edition had seven rides the PC and Mac editions never did, and one piece of scenery: the Halloween
Tower. This mod adds them to the game, each ride in its theme's ride list and the Tower in the Halloween scenery list,
built from your own PlayStation disc by the converter. The repository holds no game files: they are made on your machine
from your disc, into the data folder's assets/ps1, and the Mods box shows the mod locked until they are there.

Each ride, and the Tower, keeps the number the game saves it by (201 and up, apart from the Gold Edition's), so a park
that uses one says so in its file:

- **Loading.** A park with a PlayStation ride or the Tower loads only with the mod on and that ride or the Tower
  present. Without it the game says which rides it needs and goes back to the menu once you close the message. Nothing
  is saved over the park, so you can turn the rides on and carry on.
- **Online.** A park with PlayStation rides can be uploaded, if the server takes its rides. A player without them who
  visits it is told which rides it needs, the same way. Its page does not say so yet.
- **The PS1 tag.** In Buy Ride, the picture of each PlayStation ride has a small "PS1" tag in its top right corner, so
  you can tell them from the others. It is display only, and the launcher's Settings box turns it off with "Show Ride
  Badges" (on by default), which also turns off the "GOLD" tag of the Gold Edition's extra rides.

With the mod off the game is the original: the rides and the Tower are not in the lists, and parks that use none of them
load and save exactly as before.

## Not yet the original's

The rides' scripts, animations, sounds and run times, and the Tower's ground, the way it rises out of the ground when
built and its price, are made up where the conversion has nothing to go by. The goal is the PlayStation edition's own
behaviour; they will be brought closer to it.
