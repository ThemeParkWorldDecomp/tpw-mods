// Enable strikes: the strike_gate hook on, so that the staff room's monthly update (CStaffHQ::UpdateMonthly) considers a
// strike in an open park with visitors in it. The rest of the strike code is the original's: a strike still needs a park
// older than 24 months and staff with a grievance (CStaffHQ::ConsiderStriking), and a closed park still has none.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_enable_strikes(Hooks& hooks) { hooks.strikeGate = true; }

}  // namespace tpw::mods
