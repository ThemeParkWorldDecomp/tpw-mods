// Better land buying: the land_rough_edges, land_fair_price, land_thin_strips and land_unbought hooks on, so that a land
// purchase may run its sides across rivers, lakes and rocks inside the land, not into the cliffs at its edge
// (LLCC_IsHoardingEditLegal), may be one fence square wide, may close in land the park does not own, which then stays
// unowned and fenced until it is bought (LLCC_ReviseHoarding and LLCC_FillInsideOfPark), and costs the land inside its new
// fence line that the park does not own, rivers, lakes and rocks free. The gate's land stays outside, as in the original.
// With the mod off every purchase is the original's.
#include "mods.h"

namespace tpw::mods {

void SwitchOn_better_land_buying(Hooks& hooks) {
    hooks.landRoughEdges = true;
    hooks.landFairPrice = true;
    hooks.landThinStrips = true;
    hooks.landUnbought = true;
}

}  // namespace tpw::mods
