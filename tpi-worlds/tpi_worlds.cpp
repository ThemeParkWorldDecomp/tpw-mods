// TPI Worlds and Rides: three more worlds, Arabian Nights, Polar Zone and Land of Invention, whose content
// (mod.json's "content": the data folder's assets/tpi) the launcher's converter makes from the player's own copy of
// Theme Park Inc, the game's sequel on the same engine, and lays over the game folder with the mod on (tpw-port's
// platform/tpi_content.h).
// The tpi_worlds hook on: the game's list of worlds has the three after the original four (tpw-port's
// include/TpiWorlds.h), each played by Theme Park World's rules.
// And the tpi_islands hook on: the island screen has a second group of islands for them, with a switch between the
// groups. The original four islands stay where they are.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_tpi_worlds(Hooks& hooks) {
    hooks.tpiWorlds = true;
    hooks.tpiIslands = true;
}

}  // namespace tpw::mods
