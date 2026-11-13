# TPI Worlds and Rides

Adds three worlds made from your own copy of Theme Park Inc (SimCoaster), the game's sequel: Arabian Nights, Polar
Zone and Land of Invention, each with its own rides, shops, sideshows and scenery, played by Theme Park World's rules.
The island screen gets a second group of islands for them. It stays locked until that game's worlds are in the data
folder. A park in one of them needs them to load.

| | |
| --- | --- |
| Id | tpi-worlds |
| Version | 0.1.3 |
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
- **Shops that sell.** Theme Park Inc's shops ask up to six times what their goods cost, and its Polar Zone and Land
  of Invention records give visitors no leeway over a fair price, so in Theme Park World visitors turned most of those
  shops down. In the three worlds each shop leaves its visitors the room Theme Park World's own shops do between price
  and worth, so they buy at Theme Park Inc's prices. The photo booth ($40 a photo) is a shop visitors walk up to and
  pay, where none ever chose it; it makes them happier, and nothing shows on them afterwards. A park saved before this
  gets both when it is loaded.
- **Bridges and land.** Theme Park Inc's obstacles (the bridges, the drained lake, the tunnels, the rocks) are part of
  each world's ground, drawn as they are once cleared where the data has that state. The bridges are footbridges like
  the one in Lost Kingdom: lay a path up to each end, and the visitors cross on their way to the rides beyond; nothing
  can be built on the bridge itself. Staff walking their rounds cross them too, as the staff-cross-bridges mod lets
  them in the original worlds; in these three worlds they do so whether that mod is on or not. A park saved before this gets its bridges back as footbridges when it is loaded
  (a path laid on a bridge goes). Land past the canyons and dry riverbeds under the obstacles can be bought with Buy
  Land as any other land, so the park can grow into the land Theme Park Inc opened with its obstacles.
- **The Invention gate.** Land of Invention's gate starts closed and swings open by itself as soon as the park buys
  the land it stands on or any land right next to it with Buy Land. The park's starting fence runs through the gate,
  so the land a new park starts with does not count: only land bought beyond it does. Once the land the gate stands
  on is the park's, a path can be built through the open gate and the visitors walk through. A new or restarted park
  has it closed; a saved park needs nothing more, since the gate follows the park's land.
- **The park's edge.** Theme Park Inc shows its park's edge with each world's own walls and fences, the river and
  the lake; it draws no hoarding. Theme Park World's hoarding pieces in its data (a leafy jungle fence and a Halloween
  one) are not drawn in the three worlds, where they ran across the grass beside the worlds' own walls; a park saved
  with them loses them when it is loaded. The land the park owns and Buy Land are unchanged: the darkened ground
  outside the park still shows where it ends, and it fades into the park's land over one cell, as the ground does
  along the river's banks, where it ended in a hard stepped line.
- **Rides that run.** Theme Park Inc's Golden Ticket rides (Spin Djinn, The Scorpions and others in all three
  worlds) hold their opening pose until their script's SPECIAL switch is set, which Theme Park World never sets. In
  the three worlds such a ride's script starts with the switch set, so the ride is drawn whole once built and its
  guests board and ride, where it was left with only its moving parts showing and a queue nobody left. The Scorpions is a bumper ride of a kind Theme Park World has no bumper cars for,
  so in the three worlds it runs as Theme Park World's bumper cars, where the game crashed once it took its first
  rider. A ride's spoken lines use the donor's sound banks, which lack some of Theme Park Inc's samples; in the three
  worlds a line whose sample is missing ends quietly, where it could freeze the game. The Polar zone's ice skates have no camera
  of their own for riding them in first person; in the three worlds that view sits at the skater's head, where it did
  not open. The Great Lake's boat has a wake meant for the water of Theme Park Inc's lake; on Theme Park World's dry
  land it showed as a blue patch flickering on the grass, so in the three worlds it is hidden.
- **Outside the gate.** Theme Park Inc's entrance is laid out otherwise than Theme Park World's: the bus stops on a
  pavement two rows deep under the map's edge, with the road between it and the gate. In the three worlds the visitors
  off the bus wait to cross on that pavement and pay at the ticket booths, and the visitors leaving wait for the bus on
  it, where Theme Park World's walks would leave them standing in front of the gate. A park saved with visitors
  standing there gets them to the bus.
- **Easy mode.** A player in easy mode starts one of the three worlds with an empty park: Theme Park Inc's ready-made
  parks are not converted, and a donor's park is never used, since it would be a park of the donor world.
- **Saved games.** The master save keeps one record per world as it does for the original four, so research,
  golden keys and the park's name are kept per world. A park saved in one of the three loads only with the mod on and
  that world present. A Land of Invention park started with launcher 0.5.1 or earlier gets its staff room in the build
  list when it is loaded, and the security cameras placed in it stay cameras. With the mod off those records are passed over, so the four worlds answer the game's questions
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
