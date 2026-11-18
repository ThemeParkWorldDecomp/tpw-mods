// PS2 Guest Models (3D): its models are content (mod.json's "content": the data folder's assets/ps2, which the player's
// own PlayStation 2 disc is converted into, tpw-port's assets/ps2_convert.h), read by the launcher with the mod on.
// And the ps2_characters hook on: each person with a converted model (the eight kids, the four theme world costume
// kids, the staff and the entertainers, README.md's table) is drawn as that 3D model in the sprite pass instead of its
// sprite, at the sprite's place and height, facing where it walks, its sprite's action and frame picking the model's
// clip (tpw-port's include/Ps2Characters.h). Display only: the game's state and random numbers are not touched, so a
// park plays the same with the mod on and off.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_ps2_characters(Hooks& hooks) { hooks.ps2Characters = true; }

}  // namespace tpw::mods
