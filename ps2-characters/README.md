# PS2 Guest Models (3D)

Draws the park's children, staff and entertainers as the PlayStation 2 edition's 3D models, converted from your own
PlayStation 2 disc, walking and animated where the sprites were. It stays locked until that disc's models are in the
data folder. Display only: parks play and save exactly as before.

| | |
| --- | --- |
| Id | ps2-characters |
| Version | 0.1.1 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Unchanged |
| Hooks | ps2_characters (content: assets/ps2) |
| Needs | STP/TPW Launcher 0.6.0 or later, and your own PlayStation 2 disc |

## What it changes

The PlayStation 2 edition of the game drew its visitors as 3D models with skeletal animation, where the PC and Mac
editions draw sprites. This mod draws the PC game's children with those models, built from your own PlayStation 2 disc
by the converter. The staff and entertainers you hire are drawn with the PlayStation 2's staff and entertainer models.
The repository holds no game files: they are made on your machine from your disc, into the data
folder's assets/ps2, and the Mods box shows the mod locked until they are there.

**To get the models,** drop your PlayStation 2 disc (the disc itself, a .bin and .cue, an .iso, or a copy of its
folder) on the launcher's window, as with the other discs. The launcher checks it, converts the models and says
"PS2 Guest Models ready"; the mod is then unlocked in the Mods box. Another disc, or a damaged copy, is refused with the
reason.

- **Which models.** The PC game's eight children are drawn as the PlayStation 2's eight children, boys as boys and girls
  as girls, and the four costume children of the theme worlds as the PlayStation 2's four.
- **Staff.** Guards, handymen and researchers are drawn as the PlayStation 2's. The PlayStation 2 has one mechanic,
  so **both PC mechanics are drawn as that one mechanic**; they still are two hireable types and work as before.
- **Entertainers.** The PlayStation 2 has eight of the PC's twelve entertainers. The other four are drawn as a
  PlayStation 2 entertainer of their own theme world:

  | PC entertainer | World | Drawn as |
  | --- | --- | --- |
  | Flower | Wonder Land | Flower |
  | Gnome | Wonder Land | Gnome |
  | **Ladybird** | Wonder Land | **Flower** |
  | Dracula | Halloween | Vampire |
  | Frankenstein's monster | Halloween | Frankenstein's monster |
  | **Hunchback** | Halloween | **Frankenstein's monster** |
  | Dinosaur | Lost Kingdom | Dinosaur |
  | Explorer | Lost Kingdom | Hunter |
  | **Native** | Lost Kingdom | **Hunter** |
  | Alien | Space Zone | Alien |
  | Spaceman | Space Zone | Spaceman |
  | **Tentacle monster** | Space Zone | **Alien** |

  All 12 hireable staff and entertainer types stay, with their own names, wages and work: only how they are drawn
  changes.
- **Where.** Each person stands where its sprite stood, about as tall as its sprite, and faces the way it walks.
- **Animation.** A child's own action picks the model's: walking, running, standing, throwing up, throwing, fidgeting,
  cheering and crying. Staff and entertainers walk, stand, work (fixing, sweeping, researching, performing, the guard
  catching a thief), strike (holding up their sign, which they carry only while on strike, as on the PlayStation 2),
  and play the PlayStation 2's "picked up" clip while you hold them with the pointer. The
  walk follows the sprite's own steps, so the feet keep pace with the ground. Someone standing picks one of the
  PlayStation 2's idles.
- **Light.** The models are lit from above, and take the park's fog and the colour the sprites are drawn with. Their
  textures are drawn a little brighter, as the PlayStation 2 drew them, so they sit as bright as the sprites.
- **Heads on rides.** The visitors' heads in the ride cars, boats and seats are drawn as the head of the same child's
  model, where the sprite head was and turned with the ride. A head with no model that fits keeps its sprite.
- **First person.** Riding a ride or walking with the camcorder, the people are models too, and they stay models
  further out than in the park view, so no ring of sprites shows just past them.
- **What stays.** The shadows, picking someone with the pointer and everything they do are the game's own. Someone far
  away, too small for a model to show, keeps its sprite.

The mod changes only what is drawn. It reads where the people are and what they do, never changes it, and never uses
the game's random numbers (the idle someone picks comes from a sum of its own), so a park plays and saves the same with
the mod on and off. With the mod off the game is the original.

## Not yet

- **Theme Park Inc's visitors** (the SimCoaster/TPI mod's extra visitors) keep their sprites.
- The launcher does not yet remember the PlayStation 2 disc: if a later launcher's converter changes, drop the disc on
  it again.
