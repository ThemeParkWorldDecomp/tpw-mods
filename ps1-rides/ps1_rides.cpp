// PlayStation rides: its rides are content (mod.json's "content": the data folder's assets/ps1, which the player's own
// PlayStation disc is converted into), which the launcher lays over the game folder with the mod on (tpw-port's
// platform/content_overlay.h). The game finds them as it finds any ride, by scanning the ride folders.
// And the ps1_badge hook on: while the launcher's "Show Ride Badges" is on (its default), the Buy Ride panel's picture of
// each of these rides has a "PS1" tag in its top right corner (tpw-port's include/RideBadges.h). Display only.
// And the ps1_ground hook on: each cell of these rides' own ground is blended into the park's ground under it (tpw-port's
// CalculateSkirtVertices), as the PC rides' bases are, so the park's grass shows where the disc's grass was. Display only.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_ps1_rides(Hooks& hooks) {
    hooks.ps1Badge = true;
    hooks.ps1Ground = true;
}

}  // namespace tpw::mods
