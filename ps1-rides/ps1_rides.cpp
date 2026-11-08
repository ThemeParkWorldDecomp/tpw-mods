// PlayStation rides: no hook. Its rides are content (mod.json's "content": the data folder's assets/ps1, which the
// player's own PlayStation disc is converted into), which the launcher lays over the game folder with the mod on
// (tpw-port's platform/content_overlay.h). The game finds them as it finds any ride, by scanning the ride folders.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_ps1_rides(Hooks&) {}

}  // namespace tpw::mods
