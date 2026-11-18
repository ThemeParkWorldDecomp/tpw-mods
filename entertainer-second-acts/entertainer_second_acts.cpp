// Entertainers' second acts: the entertainer_second_acts hook on, so that an act of one of the eight costumes whose
// sprite file holds an unused second act (CEntertainer::SetState, include/EntertainerSecondActs.h) is that act or the
// first, at random from the number the original already draws for the act. The act's length, its sound when it ends,
// the crowd's reaction and the entertainer's walks, breaks and strikes are the original's.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_entertainer_second_acts(Hooks& hooks) {
    hooks.entertainerSecondActs = true;
}

}  // namespace tpw::mods
