# TPI Worlds and Rides

Adds three worlds made from your own copy of Theme Park Inc (SimCoaster), the game's sequel: Arabian Nights, Polar
Zone and Land of Invention, each with its own rides, shops, sideshows and scenery, played by Theme Park World's rules.
The island screen gets a second group of islands for them. It stays locked until that game's worlds are in the data
folder. A park in one of them needs them to load.

| | |
| --- | --- |
| Id | tpi-worlds |
| Version | 0.1.0 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Changed: a park in one of its worlds needs the mod and that world to load |
| Hooks | tpi_worlds, tpi_islands (content: assets/tpi) |
| Needs | STP/TPW Launcher 0.5.0 or later, and your own copy of Theme Park Inc (SimCoaster) |

## What it changes

Theme Park Inc, the sequel on the same engine, has three worlds Theme Park World never had: Arabian Nights, Polar Zone
and Land of Invention. This mod adds them to the game as three more worlds beside the original four, each with its own
terrain, rides, shops, sideshows, scenery and gate, built from your own copy of Theme Park Inc by the launcher's
converter. The repository holds no game files: they are made on your machine from your copy, into the data folder's
assets/tpi, and the Mods box shows the mod locked until all three worlds are there.

A converted world holds only what Theme Park Inc has. The rest of what a world needs comes from its donor, one of the
original four, which the game reads in the world's place:

| World | Title | Donor | Why that donor |
| --- | --- | --- | --- |
| arabian | Arabian Nights | jungle (Lost Kingdom) | The nearest in mood: warm, old, outdoors |
| water | Polar Zone | hallow (Halloween World) | The cold and dark world |
| science | Land of Invention | space (Space Zone) | The machines world |

The donor gives the music, the sound banks, the advisor's voice and models, and any file of the world's folder the
conversion has no source for. Rides, shops, sideshows, scenery, the island and the easy-mode park never come from the
donor: a world's build lists are its own. The advisor never names the donor: the only lines of a world's own speech
are its welcomes ("Welcome to Halloween World"), and Theme Park Inc has no welcome for its three zones, so in the mod's
worlds he says the game's own welcome that names no world instead ("Welcome to Theme Park World", or Sim Theme Park, the
one he gives a new player in the lobby). Every other line is the game's, as in the original four.

- **Three more islands.** The island screen has a second ring of islands for the three worlds, north of the original
  four. Tab, or a right click on the view, flies the camera between the rings; the arrows and the panel's buttons browse
  the ring in view. Each island is made by the converter from the world's own gate, terrain and textures. The original
  four islands stay where they are, and the mod's worlds do not move them in the Settings box's theme choice either.
- **The worlds' names.** The island screen, its gate signs and the park's name use the world's title (Arabian Nights,
  Polar Zone, Land of Invention), and each ride, shop, sideshow and piece of scenery of a converted world has the name
  Theme Park Inc itself shows for it, which the converter reads from the copy's own English object names (the game's
  built-in name table knows none of them). An item that list does not name keeps its own file's name, written in
  title case. The gate's two sign boards show the title's two halves.
- **Golden keys.** Like the original four, the three worlds cost golden keys to enter, in the order Theme Park Inc
  opens them: Land of Invention 1, Polar Zone 3 and Arabian Nights 5. The original four keep their prices.
  With the mod on, the golden key and ticket counts a player is shown also count the three worlds' records, so they
  can be higher than with the mod off (the master save itself is the same either way).
- **Prices.** As in Theme Park World, the converted rides charge nothing per ride; shops and sideshows keep Theme Park
  Inc's prices.
- **Easy mode.** A player in easy mode starts one of the three worlds with an empty park: Theme Park Inc's ready-made
  parks are not converted, and a donor's park is never used, since it would be a park of the donor world.
- **Saved games.** The master save keeps one record per world as it does for the original four, so research,
  golden keys and the park's name are kept per world. A park saved in one of the three loads only with the mod on and
  that world present. With the mod off those records are passed over, so the four worlds answer the game's questions
  (all research done, the keys collected) exactly as the original's.
- **Online.** A park in one of the three worlds cannot be published: the online server has no such world, and the
  game refuses the upload before anything is sent, with its own "cannot upload" message.

With the mod off the game is the original: the three worlds and their islands are not there, the four worlds and
their islands are untouched, and parks of the original worlds load and save exactly as before. A build without the
port's fixes never sees the converted files.

## Not yet the original's

The three worlds are played by Theme Park World's rules: its money, its visitors, its staff, its research and its
advisor. Theme Park Inc's own rules (its missions, its business goals, its staff training) are not part of this mod.
The rides' scripts and run times, the sounds and the music are the donor world's where the conversion has nothing to
go by.
