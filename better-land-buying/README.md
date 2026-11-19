# Better land buying

Land can be bought across rivers, lakes and rocks inside the park's grounds, and a strip one square wide can be bought by
itself. Land a purchase closes in stays unbought, with its fence, until you buy it, and you pay only for the land you can
build on.

| | |
| --- | --- |
| Id | better-land-buying |
| Version | 1.0.0 |
| Author | Zack Pollard |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Changed: land it left unbought inside the fence stops the original buying land |
| Hooks | land_rough_edges, land_fair_price, land_thin_strips, land_unbought |
| Needs | STP/TPW Launcher 0.6.1 or later |

## What it changes

Buy Land draws a rectangle, and the original refuses many of them. Players found two problems with that: land around a
river or a rock formation, like the big rock in the Lost Kingdom, often cannot be bought at all, and buying on two sides
of the park can leave a gap between the two that can never be closed. Both come from the original's rules for the
rectangle, and the mod changes four of them:

- **Rivers, lakes and rocks.** The original refuses a rectangle whose edge runs over two cells of rock or river bank in a
  row; open water is allowed, and rock or river inside the rectangle always was. With the mod the edge may cross them.
  The fence is drawn over them with the pieces the original already uses where a fence crosses water. The cliffs and
  mountains round the land, and the rock behind the park gate, are the park's outer walls and stay as they were: a
  rectangle still may not reach into them.
- **Strips one square wide.** The original's rectangle must be at least two fence squares wide, so a strip one square
  wide, left between two purchases, could only be bought together with land around it. With the mod one square is
  enough. The land beside the road up to the park gate is the exception: it is only half a square wide, and a fence
  there would stand against the gate, so with the mod no rectangle may reach it.
- **Land a purchase closes in.** The original refuses a rectangle that would leave land the park does not own closed in
  by the park's land. The strips above are often the only way out for land behind them, such as a stream or a lake, so
  they could not be bought without that land, and that land usually could not be bought because of its river banks.
  With the mod the purchase goes ahead and the land behind stays unbought: it stays dark, nothing can be built on it,
  and its fence stays round it until you buy it, whole or a piece at a time. Only the land inside your rectangle
  changes hands. The land in front of the park gate is the exception: a rectangle that would close it in is still
  refused.
- **The price.** The original charges for the land inside the rectangle that the park does not own, rivers and rocks
  included, and leaves out the cells under the new fence, so a strip one square wide would cost nothing. With the mod
  you pay for the land you get, inside the new fence line and the cells under it included, and rivers, lakes and rocks
  are free, as nothing can be built on them.

Everything else is the original's: the price of a cell, the bank's check that you can pay, and the rule that a rectangle
must touch the park along a side, not only at a corner.

## Details for modders

| What | Function | Mac 2.0.1 |
|---|---|---|
| The rectangle's checks and its price (the hooks land_rough_edges, land_thin_strips, land_unbought and land_fair_price) | LLCC_IsHoardingEditLegal | 0x1009464c |
| The land closed in: the image of the park, filled from outside | LLCC_SoIsItLegal | 0x10096958 |
| The purchase: the rectangle drawn as fence, the fence walk, the fill (the hook land_unbought) | LLCC_ReviseHoarding | 0x100961c0 |
| The fill: the land inside the fence walk becomes the park's, the fence off the walk is deleted (land_unbought) | LLCC_FillInsideOfPark | 0x10093b0c |
| The fence walk from the gate's right post to its left one, the park on its right | LLCC_BuildHoardingDB | 0x1008e6e0 |
| The fence pieces on water and freeform cells (mode 0x85) | CTrackCell::Modify | 0x1009dfe4 |

- **Land the park does not own** is a map cell with flag 0x40. A fence piece is a track cell of mode 0x19 standing for a
  2 x 2 block of cells; its links to the next pieces are its hoarding mask (CCellBase::GetNeighbours while gLLCC4818 is
  set). Water is map mode 2 and rock or river bank freeform, modes 7 and 0x1e (CMapCell::IsWater, IsFreeform).
- **land_rough_edges:** the side tally's run of freeform dry cells, two of which refuse the rectangle, counts only cells of
  the land's outer walls: the freeform cells joined to the edge of the map through freeform cells side by side (in all four
  parks, the cliff behind the gate and the bands along the east and south edges). A rock, a river's or a lake's banks or a
  ridge inside the land is not counted, as the tpi-worlds mod already has it for every freeform cell in its own worlds.
- **land_thin_strips:** the narrowest rectangle allowed is one fence square (the corners two cells apart) instead of two,
  and no side may stand on or beside the land in front of the gate (the columns from the left post to the right one, in
  the rows before the gate), where the land beside the road on its left is half a fence square wide.
- **land_unbought:** LLCC_SoIsItLegal's refusal is passed over unless the cells between the gate's posts in the row before
  gHoardingBottom are among those closed in. LLCC_ReviseHoarding then notes which cells the park does not own and the
  cells the purchase takes, lowX + 1 to highX and lowY + 1 to highY; LLCC_FillInsideOfPark keeps each fence piece whose
  block holds a cell staying unowned, and each corner post between two kept pieces; afterwards those cells get flag 0x40
  back, and links to pieces that are gone, or that do not link back, are taken off and their pieces re-tiled.
- **land_fair_price:** the cells charged are lox + 1 to hix and loy + 1 to hiy instead of lox + 2 to hix - 1 and loy + 2
  to hiy - 1, and a water or freeform cell is not charged.

## Saved games

A park saved with the mod on loads and plays with it off, and nothing in the file is new. But land the mod left unbought
inside the park's fence is something the original never makes, and while any is there the original refuses every land
purchase: it takes that land for land the purchase would close in. With the mod off, buy no land in such a park; turn
the mod back on, or first buy that land with it on. The launcher's Mods box says a park saved with the mod differs.

## Tests

tests/test_better_land_buying.cpp runs the port's LLCC_IsHoardingEditLegal with the mod off and on in a park drawn into
a real map grid, and tests/test_better_land_buying_fence.cpp runs the port's LLCC_FillInsideOfPark between the hook's
steps as LLCC_ReviseHoarding brackets it.
