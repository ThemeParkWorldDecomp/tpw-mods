// The tests of Better land buying's checks and price: the port's LLCC_IsHoardingEditLegal (src/game/park/LLCellSystem.cpp,
// with the image functions it calls) with the mod off and on, in parks drawn into a real map grid. The map and track cells
// lie where the game keeps them (gGameWorld + 0x2d8 and + 0x1102d8, 0x44 and 0x28 bytes a cell); the cell predicates the
// check calls are fakes that answer from the cells' fields as the game's do. A fence piece is a track cell of mode 0x19 with
// its three other cells of mode 0xc naming it as their parent, and its links only in the hoarding mask, as the game draws
// them with gLLCC4818 set: the check runs with gLLCC4818 clear, so it sees no links, as in the game.
//
// The park: the land from (22, 22) to (61, 61) is the park's, with the gate's two cells of rows 21 and 22 between its posts
// (gHoardingLeft 31 and gHoardingRight 39, gHoardingTop and gHoardingBottom 21), as LLCC_DrawInitialHoarding leaves them.
// Every 2 x 2 block (at odd x and y) that holds land of the park and land outside it is a fence piece, so the fence lies on
// the line between them as the game's does. The cases:
//
// - a band of rock (freeform, mode 7) four cells wide east of the park, inside the land, which a rectangle's sides cross:
//   refused with the mod off (two freeform dry cells in a row along a side), allowed with it on; the same band reaching the
//   edge of the map, and a cliff along the map's north edge, are the land's outer walls: refused either way; a band two cells
//   wide is allowed either way;
// - a strip one fence square wide beside the park: refused off, allowed on;
// - a notch of unowned land in the park's south side, whose mouth a rectangle closes: refused off (the land closed in),
//   allowed on;
// - land in front of the gate, between two arms of the park: a rectangle across the arms' ends, whose side runs in front of
//   the gate, refused off and on; with the land in front of the gate reached only by a corridor away from it, a rectangle
//   across the corridor that closes that land in: refused off and on (the gate's land must stay outside);
// - beside the gate (the columns of its posts and between, in the rows before it): a rectangle two squares wide with a side
//   there, allowed off (as the Mac allows it), refused on; a strip one square wide there, refused either way;
// - a park with land left unbought inside its fence, as the mod leaves it: a plain expansion elsewhere, refused off (the
//   original takes that land for land the purchase would close in), allowed on;
// - the price of a rectangle of grass with a patch of water in it: the Mac's cells (lox + 2 to hix - 1) and the water
//   charged off; the cells inside the new fence line (lox + 1 to hix) and the water free on; the bank's check either way.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include "mods.h"
#include "ActionRecorder.h"
#include "CellBase.h"
#include "LLCellSystem.h"
#include "MainBalance.h"
#include "MapCell.h"
#include "ThingReceivers.h"
#include "TrackCell.h"
#include "World.h"

// The mod's own switch-on function (better_land_buying.cpp; the launcher's catalog declares it too).
namespace tpw::mods {
void SwitchOn_better_land_buying(Hooks& hooks);
}

// The globals of other files the check reads.
CWorld* gGameWorld;
CActionRecorder gActionRecorder;
CMainBalance gMainBalance;
CMainBalance::~CMainBalance() {}
void CBalanceTable::Init() {}
const BalanceDescription* CMainBalance::GetBalanceDescription(int32_t) { return nullptr; }
BalanceValue* CMainBalance::GetBalanceValuesStart() { return mValues; }

namespace {

constexpr int kSide = 128;
constexpr int kMapCellCost = 369;   // the balance slot of a cell of land's price (Costs.MapCell)
alignas(16) uint8_t gWorldBlock[0x2d8 + 0x110000 + kSide * kSide * 0x28];
int32_t gBalance = 1000000;

uint16_t IdOf(int x, int y) { return static_cast<uint16_t>(1 + x + y * kSide); }
CMapCell* Map(int x, int y) { return reinterpret_cast<CMapCell*>(gWorldBlock + 0x2d8 + (IdOf(x, y) - 1) * 0x44); }
CTrackCell* Track(int x, int y) { return reinterpret_cast<CTrackCell*>(gWorldBlock + 0x1102d8 + (IdOf(x, y) - 1) * 0x28); }
CTrackCell* TrackOfId(uint16_t id) { return reinterpret_cast<CTrackCell*>(gWorldBlock + 0x1102d8 + (id - 1) * 0x28); }

}  // namespace

// ---- the fakes: the cell predicates, as the game's answer from the cells' fields
int32_t CMap::IsXYValid(int32_t x, int32_t y) { return x >= 0 && y >= 0 && x < kSide && y < kSide ? 1 : 0; }
int32_t CMapCell::IsFlag(uint16_t flag) const { return (mFlags & flag) != 0 ? 1 : 0; }
int32_t CMapCell::IsWater() const { return static_cast<int32_t>(mType) == 2 ? 1 : 0; }
int32_t CMapCell::IsFreeform() const { return static_cast<int32_t>(mType) == 7 || static_cast<int32_t>(mType) == 0x1e ? 1 : 0; }
int32_t CTrackCell::IsHoarding() const { return static_cast<int32_t>(mType) == 0x19 ? 1 : 0; }
int32_t CTrackCell::IsTrackX() const { return static_cast<int32_t>(mType) == 0xc || static_cast<int32_t>(mType) == 0x11 ? 1 : 0; }
uint8_t CCellBase::GetNeighbours() const { return gLLCC4818 != 0 && mKind == 2 ? mHoardingNeighbours : mNeighbours; }
int32_t CCellBase::NumberOfHVNeighbours() const {
    const uint8_t bits = GetNeighbours();
    return ((bits & 1) != 0) + ((bits & 4) != 0) + ((bits & 0x10) != 0) + ((bits & 0x40) != 0);
}
uint8_t CTrackCell::IsStraight() const {
    const uint8_t n = GetNeighbours();
    if (NumberOfHVNeighbours() != 2) return 0;
    return ((n & 1) != 0 && (n & 0x10) != 0) || ((n & 4) != 0 && (n & 0x40) != 0) ? n : 0;
}
CMapCell* CCellBase::GetMapCellEquiv() { return reinterpret_cast<CMapCell*>(gWorldBlock + 0x2d8 + (mCellId - 1) * 0x44); }
const CMapCell* CCellBase::GetMapCellEquiv() const { return reinterpret_cast<const CMapCell*>(gWorldBlock + 0x2d8 + (mCellId - 1) * 0x44); }
CBankAccount* CWorld::GetBankAccount() { return reinterpret_cast<CBankAccount*>(&gBalance); }
int32_t CBankAccount::GetBalance() const { return gBalance; }

namespace {

int gFailures = 0;

void Check(bool ok, const std::string& what) {
    std::printf("%s %s\n", ok ? "ok  " : "FAIL", what.c_str());
    if (!ok) ++gFailures;
}

// The land: owned[y][x] true for the park's cells; terrain[y][x] the map cell's mode (0 grass, 2 water, 7 rock).
bool gOwned[kSide][kSide];
int gTerrain[kSide][kSide];

void ClearLand() {
    std::memset(gOwned, 0, sizeof gOwned);
    std::memset(gTerrain, 0, sizeof gTerrain);
}

void Own(int x0, int y0, int x1, int y1, bool owned = true) {
    for (int y = y0; y <= y1; ++y)
        for (int x = x0; x <= x1; ++x) gOwned[y][x] = owned;
}

void Terrain(int x0, int y0, int x1, int y1, int mode) {
    for (int y = y0; y <= y1; ++y)
        for (int x = x0; x <= x1; ++x) gTerrain[y][x] = mode;
}

// The park of every case, before its own changes: (22, 22) to (61, 61), and the gate's cells between the posts.
void BasePark() {
    ClearLand();
    Own(22, 22, 61, 61);
    Own(33, 21, 38, 21);
}

// The cells from the land, and a fence piece on every block (odd x and y) that holds both the park's land and land outside.
void Build() {
    std::memset(gWorldBlock, 0, sizeof gWorldBlock);
    gGameWorld = reinterpret_cast<CWorld*>(gWorldBlock);
    for (int y = 0; y < kSide; ++y) {
        for (int x = 0; x < kSide; ++x) {
            CMapCell* map = Map(x, y);
            map->mCellId = IdOf(x, y);
            map->mKind = 1;
            map->mType = static_cast<CELL_MODES>(gTerrain[y][x]);
            map->mFlags = gOwned[y][x] ? 0 : 0x40;
            CTrackCell* track = Track(x, y);
            track->mCellId = IdOf(x, y);
            track->mKind = 2;
        }
    }
    for (int y = 1; y + 1 < kSide; y += 2) {
        for (int x = 1; x + 1 < kSide; x += 2) {
            const int owned = gOwned[y][x] + gOwned[y][x + 1] + gOwned[y + 1][x] + gOwned[y + 1][x + 1];
            if (owned == 0 || owned == 4) continue;
            Track(x, y)->mType = static_cast<CELL_MODES>(0x19);
            for (const int d : {1, kSide, kSide + 1}) {
                CTrackCell* part = TrackOfId(static_cast<uint16_t>(IdOf(x, y) + d));
                part->mType = static_cast<CELL_MODES>(0xc);
                part->mParentID = IdOf(x, y);
            }
        }
    }
    gLLCCWidth = kSide;
    gLLCCHeight = kSide;
    gHoardingLeft = 31;
    gHoardingTop = 21;
    gHoardingRight = 39;
    gHoardingBottom = 21;
    gLLCC4818 = 0;
    gActionRecorder.mRecording = 0;
    gMainBalance.mValues[kMapCellCost].mInteger = 100;
    gBalance = 1000000;
}

void SetMod(bool on) {
    tpw::mods::Hooks hooks;
    if (on) tpw::mods::SwitchOn_better_land_buying(hooks);
    tpw::mods::Apply(hooks);
}

// The check as the buy land tool makes it: the image of the park at the first click, then the rectangle, charged.
bool Legal(int x1, int y1, int x2, int y2, int32_t* cost = nullptr) {
    LLCC_CreateCurrentHoardingImage();
    gLLCCC204 = 0;
    const int32_t answer = LLCC_IsHoardingEditLegal(x1, y1, x2, y2, true);
    if (cost != nullptr) *cost = gLLCC47CC;
    return answer != 0;
}

void BothWays(const std::string& name, int x1, int y1, int x2, int y2, bool off, bool on) {
    SetMod(false);
    Check(Legal(x1, y1, x2, y2) == off, name + ": mod off, " + (off ? "allowed" : "refused"));
    SetMod(true);
    Check(Legal(x1, y1, x2, y2) == on, name + ": mod on, " + (on ? "allowed" : "refused"));
}

}  // namespace

int main() {
    // The switch-on function sets the four hooks and nothing else.
    {
        tpw::mods::Hooks hooks;
        tpw::mods::SwitchOn_better_land_buying(hooks);
        Check(hooks.landRoughEdges && hooks.landFairPrice && hooks.landThinStrips && hooks.landUnbought,
              "the mod switches on land_rough_edges, land_fair_price, land_thin_strips and land_unbought");
        Check(!hooks.strikeGate && !hooks.strikeMessages && !hooks.staffBridgeWander && !hooks.ps1Badge && !hooks.ps1Ground &&
                  !hooks.ps1SaveRepair && !hooks.ps1PickRotation && !hooks.tpiWorlds && !hooks.tpiIslands,
              "the mod switches on no other hook");
    }

    // Rock beside the park: the sides of (61, 31) - (81, 51) cross a band four cells wide at x 70 to 73, inside the land (rows
    // 10 to 117) or reaching the map's edges (a wall).
    BasePark();
    Build();
    BothWays("grass east of the park", 61, 31, 81, 51, true, true);
    Terrain(70, 10, 73, kSide - 11, 7);
    Build();
    BothWays("sides across a band of rock four cells wide inside the land", 61, 31, 81, 51, false, true);
    BasePark();
    Terrain(70, 0, 73, kSide - 1, 7);
    Build();
    BothWays("sides across a band of rock four cells wide reaching the map's edge (a wall)", 61, 31, 81, 51, false, false);
    BasePark();
    Terrain(70, 0, 71, kSide - 1, 7);
    Build();
    BothWays("sides across a band of rock two cells wide", 61, 31, 81, 51, true, true);

    // A cliff along the map's north edge, rows 0 to 12: a rectangle from the park's north side into it, and one beside it.
    BasePark();
    Terrain(0, 0, kSide - 1, 12, 7);
    Build();
    BothWays("sides into the cliff along the map's edge (a wall)", 41, 5, 61, 21, false, false);
    BothWays("grass between the park and the cliff", 41, 13, 61, 21, true, true);

    // A strip one fence square wide beside the park, and two squares wide.
    BasePark();
    Build();
    BothWays("a strip one fence square wide", 61, 31, 63, 51, false, true);
    BothWays("a strip two fence squares wide", 61, 31, 65, 51, true, true);
    BothWays("a rectangle touching the park at one corner only", 61, 61, 71, 71, false, false);

    // A notch of unowned land in the park's south side, x 42 to 53 from y 42: (41, 55) - (53, 61) closes its mouth.
    BasePark();
    Own(42, 42, 53, 61, false);
    Build();
    BothWays("a rectangle that closes in a notch of unowned land", 41, 55, 53, 61, false, true);
    BothWays("a rectangle that fills the whole notch", 41, 41, 53, 61, true, true);

    // Two arms of the park north of it, either side of the gate (x 22 to 29 and 42 to 61, y 6 to 21): (29, 1) - (43, 5)
    // closes in the land in front of the gate.
    BasePark();
    Own(22, 6, 29, 21);
    Own(42, 6, 61, 21);
    Build();
    BothWays("a rectangle across the arms' ends, in front of the gate", 29, 1, 43, 5, false, false);
    BothWays("a rectangle on one arm's end, the gate's land still open", 41, 1, 61, 5, true, true);
    // (A rectangle ending above the left post, at row 19, the Mac refuses too: the post is cut off from the land outside.)
    BothWays("a rectangle two squares wide beside the gate", 25, 11, 31, 17, true, false);
    BothWays("a strip one square wide beside the gate", 29, 11, 31, 19, false, false);

    // The land in front of the gate (x 30 to 41, y 6 to 20) closed above by a bar of the park's (y 2 to 5) and reached from
    // outside only by a corridor along rows 6 to 9, west of the gate's columns: (21, 5) - (29, 11) fills the corridor's mouth.
    BasePark();
    Own(22, 2, 61, 5);
    Own(22, 10, 29, 21);
    Own(42, 6, 61, 21);
    Build();
    BothWays("a rectangle across a corridor that closes in the land in front of the gate", 21, 5, 29, 11, false, false);
    BothWays("a rectangle beside the park that leaves the corridor open", 13, 13, 21, 19, true, true);

    // Land left unbought inside the fence (x 42 to 53, y 42 to 53, closed all round by the park), as the mod leaves a pocket:
    // a plain expansion east of the park.
    BasePark();
    Own(42, 42, 53, 53, false);
    Build();
    BothWays("a plain expansion in a park with land left unbought inside its fence", 61, 31, 71, 41, false, true);

    // The price of (61, 31) - (71, 41), grass with water at (65, 35) - (66, 36).
    BasePark();
    Terrain(65, 35, 66, 36, 2);
    Build();
    int32_t cost = 0;
    SetMod(false);
    Check(Legal(61, 31, 71, 41, &cost) && cost == 8 * 8 * 100, "price, mod off: the cells from x 63 to 70 and y 33 to 40, water too");
    SetMod(true);
    Check(Legal(61, 31, 71, 41, &cost) && cost == (10 * 10 - 4) * 100, "price, mod on: the cells from x 62 to 71 and y 32 to 41, water free");
    Check(Legal(61, 31, 63, 51, &cost) && cost == 2 * 20 * 100, "price, mod on: a strip one square wide costs its two columns");
    gBalance = 9000;
    Check(!Legal(61, 31, 71, 41) && gLLCCC204 == 1, "price, mod on: refused when the bank cannot pay it");
    gBalance = 9600;
    Check(Legal(61, 31, 71, 41), "price, mod on: allowed when the bank can pay it exactly");
    SetMod(false);
    gBalance = 6400;
    Check(Legal(61, 31, 71, 41), "price, mod off: allowed when the bank can pay the Mac's price exactly");

    // A rock in the rectangle is free with the mod on, and the band of rock across the sides costs nothing.
    BasePark();
    Terrain(70, 10, 73, kSide - 11, 7);
    Build();
    SetMod(true);
    Check(Legal(61, 31, 81, 51, &cost) && cost == (20 * 20 - 4 * 20) * 100, "price, mod on: the rock in the rectangle is free");

    SetMod(false);
    std::printf("%s: %d failure(s)\n", gFailures == 0 ? "PASS" : "FAIL", gFailures);
    return gFailures == 0 ? 0 : 1;
}
