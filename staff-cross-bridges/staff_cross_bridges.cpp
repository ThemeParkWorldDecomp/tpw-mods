// Patrolling staff can cross bridges: the staff_bridge_wander hook on, so that a staff member's random walk
// (CPeep::SetRandomDestination) may step from a path onto an adjacent footbridge cell, along the bridge and off it onto a
// path again, as a walk to a target already may. The rest of the walk is the original's: the patrol area, never straight
// back, one to five steps and a random spot in the last cell. Visitors walk as before.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_staff_cross_bridges(Hooks& hooks) {
    hooks.staffBridgeWander = true;
}

}  // namespace tpw::mods
