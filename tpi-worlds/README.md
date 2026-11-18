# SimCoaster/TPI - New Worlds, Rides and Sprites

Adds three worlds made from your own copy of SimCoaster (AKA Theme Park Inc, 2001), the game's sequel: Arabian Nights, Polar
Zone and Land of Invention, each with its own rides, shops, sideshows and scenery, played by Theme Park World's rules.
The island screen gets a second group of islands for them. It stays locked until that game's worlds are in the data
folder. A park in one of them needs them to load.

| | |
| --- | --- |
| Id | tpi-worlds |
| Version | 0.1.7 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Changed: a park in one of its worlds needs the mod and that world to load |
| Hooks | tpi_worlds, tpi_islands (content: assets/tpi) |
| Needs | STP/TPW Launcher 0.5.0 or later, and your own copy of SimCoaster AKA Theme Park Inc |

## What it changes

SimCoaster AKA Theme Park Inc, the sequel on the same engine, has three worlds Theme Park World never had: Arabian Nights, Polar Zone
and Land of Invention. This mod adds them to the game as three more worlds beside the original four, each with its own
terrain, rides, shops, sideshows, scenery and gate, built from your own copy of the sequel by the launcher's
converter. The repository holds no game files: they are made on your machine from your copy, into the data folder's
assets/tpi, and the Mods box shows the mod locked until all three worlds are there.

A converted world holds only what SimCoaster AKA Theme Park Inc has. The rest of what a world needs comes from its donor, one of the
original four, which the game reads in the world's place:

| World | Title | Donor | Why that donor |
| --- | --- | --- | --- |
| arabian | Arabian Nights | jungle (Lost Kingdom) | The nearest in mood: warm, old, outdoors |
| water | Polar Zone | hallow (Halloween World) | The cold and dark world |
| science | Land of Invention | space (Space Zone) | The machines world |

The donor gives the advisor's voice and models, and any file of the world's folder the conversion has no source for.
Rides, shops, sideshows, scenery, the island and the easy-mode park never come from the donor: a world's build lists
are its own. Nor do the world's sounds and music: its ambience, its rides' sounds and its music are its own
SimCoaster AKA Theme Park Inc ones, and where that game has none (the island screen's sound effects) it plays nothing.
The sounds every world shares (the visitors, the staff, the interface) are Theme Park World's, as in the original four. The advisor never names the donor: the only lines of a
world's own speech are its welcomes ("Welcome to Halloween World"), and SimCoaster AKA Theme Park Inc has no welcome for its three
zones, so in the mod's worlds he says the game's own welcome that names no world instead ("Welcome to Theme Park
World", or Sim Theme Park, the one he gives a new player in the lobby). Every other line is the game's, as in the
original four.

- **Three more islands.** The island screen has a second ring of islands for the three worlds, north of the original
  four. Tab, or a right click on the view, flies the camera between the rings; the arrows and the panel's buttons browse
  the ring in view. Each island is made by the converter from the world's own gate, terrain and textures. The original
  four islands stay where they are, and the mod's worlds do not move them in the Settings box's theme choice either.
- **Each island's own music.** With one of the three islands chosen, the island screen plays a loop of that world's
  own SimCoaster AKA Theme Park Inc park music (the music the park itself plays), where it played its donor's island
  music. That game has no island screen, so the converter makes it as Theme Park World's own island music was made:
  three segments of one passage of the world's park music, all of one length, played one after another in a random
  order. Which three is worked out from your copy's music files: the passage with the most segments of one length
  between 15 and 25 seconds. Worlds made by an older launcher are converted again to get this.
- **No donor sounds on the island screen.** With one of the three islands chosen, the island screen no longer plays
  its donor's sound effects (Lost Kingdom's jungle beasts on Arabian Nights, Halloween World's bats and bells on Polar
  Zone, Space Zone's spaceships on Land of Invention). SimCoaster AKA Theme Park Inc has no island screen and no such
  sounds, so those islands play their music and the island screen's own sounds (the thunder and the rest that every
  island has), which are the same in both games.
- **No more fast repeats in the park music.** In a busy Arabian Nights park (and a very busy Polar Zone one) the music
  turned into one short burst of noise repeated many times a second. SimCoaster AKA Theme Park Inc's own music files
  for those two worlds end early (in every copy of the game): Arabian Nights' lists 143 pieces of music and has 103,
  Polar Zone's lists 116 and has 101, and the busiest park music is made of the missing ones. With the mod, a missing
  piece is never played: the music plays another piece of the same level, or the level below when the whole level is
  missing. Land of Invention's music is whole.
- **Waves round the islands.** The three islands have the lapping waves the original four have: a band of the island
  screen's own wave texture round each island's shore, the same width, spacing and speed as on the original islands,
  following the island's outline. The converter makes them from your copy of Theme Park World's island screen
  (lobby.wad), so nothing of either game is in the mod.
- **Open water.** The island screen's sea reaches further, so the second ring of islands sits in open water and the
  sea's edge never comes into view as the camera circles an island or flies between the rings. The sea runs on with
  the original's texture, colour, movement, light and fog, fading into the distance as the original does. The
  original four islands are unchanged. Worlds made by an older launcher are converted again to get the waves and
  the sea.
- **The worlds' names.** The island screen, its gate signs and the park's name use the world's title (Arabian Nights,
  Polar Zone, Land of Invention), and each ride, shop, sideshow and piece of scenery of a converted world has the name
  SimCoaster AKA Theme Park Inc itself shows for it, which the converter reads from the copy's own English object names (the game's
  built-in name table knows none of them). An item that list does not name keeps its own file's name, written in
  title case. The gate's two sign boards show the title's two halves.
- **Golden keys.** Like the original four, the three worlds cost golden keys to enter, in the order SimCoaster AKA Theme Park Inc
  opens them: Land of Invention 1, Polar Zone 3 and Arabian Nights 5. The original four keep their prices.
  With the mod on, the golden key and ticket counts a player is shown also count the three worlds' records, so they
  can be higher than with the mod off (the master save itself is the same either way).
- **Golden tickets.** Each of the three worlds asks for its six golden tickets what the Theme Park World world of the
  same key price asks: Land of Invention what Lost Kingdom asks, Polar Zone what Wonder Land asks and Arabian Nights
  what Space Zone asks (so many visitors in all, so many in the park at once, so many happy, the year's profit and the
  recent visitors), read from your own Theme Park World data when the world is converted. SimCoaster AKA Theme Park Inc's own world
  files have those lines only as comments, and before this the three worlds asked for nothing, so their tickets, and
  the keys they add up to, came at the first checks after the gates opened. Tickets already won in a saved park stay
  won. Worlds made by an older launcher are converted again to get this.
- **Prices.** As in Theme Park World, the converted rides charge nothing per ride; shops and sideshows keep SimCoaster AKA Theme Park
  Inc's prices.
- **Shops that sell.** SimCoaster AKA Theme Park Inc's shops ask up to six times what their goods cost, and its Polar Zone and Land
  of Invention records give visitors no leeway over a fair price, so in Theme Park World visitors turned most of those
  shops down. In the three worlds each shop leaves its visitors the room Theme Park World's own shops do between price
  and worth, so they buy at those prices. The photo booth ($40 a photo) is a shop visitors walk up to and
  pay, where none ever chose it; it makes them happier, and nothing shows on them afterwards. A park saved before this
  gets both when it is loaded.
- **Bridges and land.** SimCoaster AKA Theme Park Inc's obstacles (the bridges, the drained lake, the tunnels, the rocks) are part of
  each world's ground, drawn as they are once cleared where the data has that state. The bridges are footbridges like
  the one in Lost Kingdom: lay a path up to each end, and the visitors cross on their way to the rides beyond; nothing
  can be built on the bridge itself. Staff walking their rounds cross them too, as the staff-cross-bridges mod lets
  them in the original worlds; in these three worlds they do so whether that mod is on or not. A park saved before this gets its bridges back as footbridges when it is loaded
  (a path laid on a bridge goes). Land past the canyons and dry riverbeds under the obstacles can be bought with Buy
  Land as any other land, so the park can grow into the land the sequel opened with its obstacles.
- **The Invention gate.** Land of Invention's gate starts closed and swings open by itself as soon as the park buys
  the land it stands on or any land right next to it with Buy Land. The park's starting fence runs through the gate,
  so the land a new park starts with does not count: only land bought beyond it does. Once the land the gate stands
  on is the park's, a path can be built through the open gate and the visitors walk through. A new or restarted park
  has it closed; a saved park needs nothing more, since the gate follows the park's land.
- **The tunnels.** Arabian Nights' and Polar Zone's tunnels are drawn dug through, and the visitors walk along them
  like a footbridge: lay a path up to each end. While the land at a tunnel's far end is not the park's, the rubble that
  blocked it in SimCoaster AKA Theme Park Inc still lies there; it goes as soon as the park buys land on the tunnel or right next to
  it beyond its starting fence, as the Invention gate opens. A saved park needs nothing more.
- **The rock.** Arabian Nights' great rock in the northwest stands, and nothing can be built under it, until the park
  buys land on it or right next to it beyond its starting fence; then it clears and its ground is land like any other.
  A saved park needs nothing more: the rock follows the park's land.
- **The cliff and the swamp.** Arabian Nights' cliff in the east and Polar Zone's swamp beside its starting land stay
  the same way, until the park buys land on them or right next to them beyond its starting fence; then they clear, the
  cliff's ground becoming land like any other. SimCoaster AKA Theme Park Inc cleared each with a mission Theme Park World has no part
  of (golden tickets and skilled staff); buying the land stands in for it. The swamp lies in the ground in the
  northwest corner as the sequel draws it, and nothing is built on it until it clears (since launcher 0.5.6;
  before, it was not drawn at all and blocked nothing).
- **The park's edge.** SimCoaster AKA Theme Park Inc shows its park's edge with each world's own walls and fences, the river and
  the lake; it draws no hoarding. Theme Park World's hoarding pieces in its data (a leafy jungle fence and a Halloween
  one) are not drawn in the three worlds, where they ran across the grass beside the worlds' own walls; a park saved
  with them loses them when it is loaded. The land the park owns and Buy Land are unchanged: the darkened ground
  outside the park still shows where it ends, and it fades into the park's land over one cell, as the ground does
  along the river's banks, where it ended in a hard stepped line.
- **Rides that run.** SimCoaster AKA Theme Park Inc's Golden Ticket rides (Spin Djinn, The Scorpions and others in all three
  worlds) hold their opening pose until their script's SPECIAL switch is set, which Theme Park World never sets. In
  the three worlds such a ride's script starts with the switch set, so the ride is drawn whole once built and its
  guests board and ride, where it was left with only its moving parts showing and a queue nobody left. Such a ride
  built before this (in a park saved with 0.5.1) gets the switch when the park is loaded and starts running.
  The Scorpions is a bumper ride of a kind Theme Park World has no bumper cars for,
  so in the three worlds it runs as Theme Park World's bumper cars, where the game crashed once it took its first
  rider. A park saved with The Scorpions before launcher 0.5.2 loads with it as those bumper cars, where it crashed
  as it loaded. A ride's spoken lines use the donor's sound banks, which lack some of the sequel's samples; in the three
  worlds a line whose sample is missing ends quietly, where it could freeze the game. The Polar zone's ice skates have no camera
  of their own for riding them in first person; in the three worlds that view sits at the skater's head, where it did
  not open. The Great Lake is the sequel's boating lake on Land of Invention's lake: it
  can be built only with its back (the side away from its entrance and exit) on the lake's water, so the lake's shore
  land must be bought first. Its boats sail out onto the lake, bumping about among the buoys with their wake on the
  water, and come back to the ride to let their riders off; the riders stay seated, and the first person view follows
  a boat. A Great Lake built away from the water in a park saved before 0.1.6 keeps its boats on its own plot, as
  before. The boats float a little above the water or the ground, so their wake never flickers.
- **Coasters at full speed.** SimCoaster AKA Theme Park Inc's coasters and flumes have speed limits of their own, lower than Theme
  Park World's (at the speed a ride is built with, 80 for most coasters, 56 for some flumes, 44 for
  two coasters, where Theme Park World's own coasters go up to 110). On Theme Park World's track a train held to that
  limit stopped gaining speed down a drop, came out of it slow, and every climb and loop after that needed a chain
  lift. The converter leaves those limits out, so the three worlds' coasters and flumes run as fast as Theme Park
  World's own and keep their speed through drops and loops. The ride's speed setting still works as in Theme Park
  World. A coaster built before this runs at the new speeds once the worlds are converted again (the launcher does
  that by itself when it updates them).
- **The Depth Charge flume.** The Bonus Disk's Depth Charge flume has its water and its belt the other way round from
  every other flume, so in Theme Park World it showed the lift belt on its flat and downhill track and water on the
  climbs, and the belt over the whole of each lift hill, down its far side too. The converter swaps the two and leaves
  out its own lift section, so it has water on the flat and the drops and the belt on the climbs like the other
  flumes.
- **Special rides never wear out.** SimCoaster AKA Theme Park Inc's Golden Ticket rides (its special rides: Palace Of Delights, Spin
  Djinn, The Scarabs and The Scorpions in Arabian Nights, Drop In The Ocean, Hasty Husky, Reindeer Run and Whale Of A
  Time in Polar Zone, The Thrill Mill, High Flyer, The Great Lake and Wheel Deal in Land of Invention) never wear out
  there, as they cannot be rebuilt. In the three worlds they do not wear either: they keep their state of repair and
  their remaining life, so they never break down from wear or need a mechanic's repair, where they wore out like any
  other ride. Such a ride in a park saved before launcher 0.5.6 gets its remaining life back when the park is loaded.
  Worlds made by an older launcher are converted again to get this.
- **Outside the gate.** SimCoaster AKA Theme Park Inc's entrance is laid out otherwise than Theme Park World's: the bus stops on a
  pavement two rows deep under the map's edge, with the road between it and the gate. In the three worlds the visitors
  off the bus wait to cross on that pavement and pay at the ticket booths, and the visitors leaving wait for the bus on
  it, where Theme Park World's walks would leave them standing in front of the gate. A park saved with visitors
  standing there gets them to the bus.
- **The scenery round the park.** SimCoaster AKA Theme Park Inc places its own scenery outside each park when a park starts: the
  waterfall and the water of the ponds by the entrance road, the ticket booths, the helipad, the bunting, and each
  world's installations, penguins or seals. In the three worlds it is placed the same way, where it was missing and the
  ponds' rock pits showed flat cyan or brown. A park saved before launcher 0.5.6 gets the pieces it lacks when it is
  loaded. Worlds made by an older launcher are converted again to get this.
- **SimCoaster AKA Theme Park Inc's visitors.** The sequel's own kids, parents and grandparents walk and ride among Theme Park
  World's kids in the three worlds: 7 visitors in 10 look like kids, 2 like parents and 1 like a grandparent, as there.
  The kids are six of Theme Park World's eight and the sequel's four, each as often as the others:
  Theme Park World's other two look the same as two of those four, which take their place in these worlds. A
  visitor keeps its look when the park is saved and loaded; with the mod off, or in a build without it, the same
  park's visitors are all Theme Park World's kids.
- **SimCoaster AKA Theme Park Inc's three generations.** Each visitor in the three worlds also acts as one of the sequel's own visitor
  types of the generation it looks like (eight each for kids, parents and grandparents), taken from your
  copy's levels file: it arrives with that type's money (spread either way as in the sequel), it prefers rides as
  exciting as that type likes them, so most grandparents favour the gentler ones, and it walks at that type's speed,
  so grandparents walk at under half a kid's pace and parents a little slower than kids. Every few days, as the sequel does, the
  weather moves each generation's happiness, thirst and hunger its own way: rain, storms, snow and
  heatwaves (a heatwave being the world's own hottest weather; snow counted as rain until launcher 0.5.6). Which type
  a visitor is follows from the visitor itself, so it stays the same when the park is saved and loaded, and nothing
  new goes into the save. The sequel's umbrellas and its boredom threshold have nothing in Theme Park World to act
  on and are left out. Worlds made by an older launcher are converted again to get this.
- **Men's and women's toilets.** SimCoaster AKA Theme Park Inc has a toilet for boys and one for girls. In the three worlds, once the
  park has at least one of each (open or not), men and boys use only the men's and women and girls only the women's;
  with only one kind built, everyone uses it. A visitor is a boy or a girl, a man or a woman, by its look. Theme Park
  World's own toilet is for everyone. Nothing is saved: an older park follows the rule as soon as it is loaded.
- **Easy mode.** A player in easy mode starts one of the three worlds with an empty park: SimCoaster AKA Theme Park Inc's ready-made
  parks are not converted, and a donor's park is never used, since it would be a park of the donor world.
- **Saved games.** The master save keeps one record per world as it does for the original four, so research,
  golden keys and the park's name are kept per world. A park saved in one of the three loads only with the mod on and
  that world present. A Land of Invention park started with launcher 0.5.1 or earlier gets its staff room in the build
  list when it is loaded, and the security cameras placed in it stay cameras. With the mod off those records are passed over, so the four worlds answer the game's questions
  (all research done, the keys collected) exactly as the original's.
- **Online.** A park in one of the three worlds cannot be published: the online server has no such world, and the
  game refuses the upload before anything is sent, with its own "cannot upload" message.

With the mod off the game is the original: the three worlds and their islands are not there, the four worlds,
their islands and the island screen's sea are untouched, and parks of the original worlds load and save exactly as before. A build without the
port's fixes never sees the converted files.

## Not yet the original's

The three worlds are played by Theme Park World's rules: its money, its staff, its research and its advisor, and its
visitors but for the three generations above. SimCoaster AKA Theme Park Inc's own rules (its missions, its business goals, its staff
training) are not part of this mod. The rides' scripts and run times are the donor world's where the
conversion has nothing to go by. The world's sounds and music, in the park and on the island screen, are its own; the
sounds every world shares and the advisor's voice are Theme Park World's.
