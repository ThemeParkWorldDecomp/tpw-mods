// PlayStation rides: its rides are content (mod.json's "content": the data folder's assets/ps1, which the player's own
// PlayStation disc is converted into), which the launcher lays over the game folder with the mod on (tpw-port's
// platform/content_overlay.h). The game finds them as it finds any ride, by scanning the ride folders.
// And the ps1_badge hook on: while the launcher's "Show Ride Badges" is on (its default), the Buy Ride panel's picture of
// each of these rides has a "PS1" tag in its top right corner (tpw-port's include/RideBadges.h). Display only.
// And the ps1_ground hook on: each cell of these rides' own ground is blended into the park's ground under it (tpw-port's
// CalculateSkirtVertices), as the PC rides' bases are, so the park's grass shows where the disc's grass was. Display only.
// And the ps1_save_repair hook on: as a park loads, each of these rides whose saved doors are not where its ride file now
// puts them has them moved, with its queue and exit path joined again (tpw-port's include/Ps1SaveRepair.h).
// And the ps1_pick_rotation hook on: each of these rides picked from Buy Ride starts half a turn round, so that its
// entrance comes up at the bottom at the default camera, as a PC ride's does (tpw-port's include/Ps1PickRotation.h).
// Only the pick changes: saved parks and the rides already placed keep their rotation.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_ps1_rides(Hooks& hooks) {
    hooks.ps1Badge = true;
    hooks.ps1Ground = true;
    hooks.ps1SaveRepair = true;
    hooks.ps1PickRotation = true;
}

}  // namespace tpw::mods
