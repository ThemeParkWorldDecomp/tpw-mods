// The tests of Patrolling staff can cross bridges: a peep's random walk (CPeep::SetRandomDestination, the port's
// src/game/people/Peep.cpp) with the mod off and on, in a small park drawn into a real map grid. The functions it calls
// are fakes here: the cell predicates answer from the cells' modes as the game's do, the random numbers come from a fixed
// generator that counts its draws, the patrol area is a box, and SetDest records the target, where the next walk starts.
//
// The park: path on both banks of a stream, joined by a four cell footbridge (mode 0x1e, as CMapCell::Reset makes the
// Lost Kingdom's), and a row of the pavement outside the park gate (also mode 0x1e, with flag 0x80) beside a path.
//
// Mod off: no walk ever steps onto the bridge (tpw-decomp's tests/test_CPeep_SetRandomDestination.py compares the walk
// with the Mac's program). Mod on: a cleaner steps onto the bridge, walks across and steps off on the far bank; a bridge
// outside the patrol area is never taken; visitors walk as before; and in a park with no bridge, every walk and every
// random draw is the same as with the mod off.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <string>
#include <vector>

#include "mods.h"
#include "CellBase.h"
#include "Debug.h"
#include "MapCell.h"
#include "ObjectParts.h"
#include "ThingReceivers.h"
#include "World.h"

// The mod's own switch-on function (staff_cross_bridges.cpp; the launcher's catalog declares it too).
namespace tpw::mods {
void SwitchOn_staff_cross_bridges(Hooks& hooks);
}

// The globals of other files the walk reads.
CWorld* gWorld;
CMap* gWorldMap;
extern const MapDelta gMapDeltaPlusX = {1, 0, 0};
extern const MapDelta gMapDeltaPlusY = {0, 1, 0};
extern const MapDelta gMapDeltaMinusY = {0, -1, 0};
extern const MapDelta gMapDeltaMinusX = {-1, 0, 0};

namespace {

// One walk: the cells it stepped through (each step's cell, as GetNeighbours is asked), its target and what else it did.
struct Walk {
    std::vector<uint16_t> cells;
    std::vector<uint32_t> draws;      // the random numbers it drew, in order
    std::vector<std::string> other;   // SetDest by id, FindDestInPatrolArea, the traces and bubbles
    uint16_t target = 0;              // the cell SetDest was given (0: none)
    int32_t result = 0;
};

Walk gWalk;
uint32_t gSeed = 1;
int gPatrol[4] = {0, 0, 127, 127};   // the patrol box: x0, y0, x1, y1 (inclusive)
uint16_t gPatrolHome = 0;            // where FindDestInPatrolArea sends the peep

uint16_t IdOf(int x, int y) { return static_cast<uint16_t>(x + y * 128 + 1); }
int XOf(uint16_t id) { return (id - 1) & 0x7f; }
int YOf(uint16_t id) { return (id - 1) >> 7; }
CMapCell* Cell(uint16_t id) {
    return reinterpret_cast<CMapCell*>(reinterpret_cast<uint8_t*>(gWorldMap) + (static_cast<int32_t>(id) - 1) * 0x44);
}
uint16_t IdOfCell(const void* cell) {
    return static_cast<uint16_t>((reinterpret_cast<const uint8_t*>(cell) - reinterpret_cast<const uint8_t*>(gWorldMap)) / 0x44 + 1);
}

}  // namespace

// ---- the fakes
bool CPeep::IsStranded() { return false; }
bool CPeep::IsLocalAreaUnchangedSinceBecomingStranded() { return false; }
void CPeep::SetStranded(int32_t stranded) {
    if (stranded != 0) gWalk.other.push_back("SetStranded");
}
int32_t CPeep::SetDest(tag_IDX_TYPE<IDXTYPEUniqueNameMapId, uint16_t> cell) {
    gWalk.other.push_back("SetDest " + std::to_string(cell.mIndex));
    gWalk.target = cell.mIndex;
    return 1;
}
int32_t CPeep::SetDest(MapPos x, MapPos y) {
    gWalk.target = IdOf(x.mCell, y.mCell);
    return 1;
}
void CThoughts::ShowThoughtBubble(ThoughtType type, bool always) { gWalk.other.push_back("ShowThoughtBubble"); }
uint32_t CWorld::GetRand() {
    gSeed ^= gSeed << 13;
    gSeed ^= gSeed >> 17;
    gSeed ^= gSeed << 5;
    gWalk.draws.push_back(gSeed);
    return gSeed;
}
int32_t CStaff::InsidePatrolArea(tag_IDX_TYPE<IDXTYPEUniqueNameMapId, uint16_t> cell) const {
    const int x = XOf(cell.mIndex);
    const int y = YOf(cell.mIndex);
    return x >= gPatrol[0] && y >= gPatrol[1] && x <= gPatrol[2] && y <= gPatrol[3] ? 1 : 0;
}
int32_t CStaff::FindDestInPatrolArea() {
    gWalk.other.push_back("FindDestInPatrolArea");
    gWalk.target = gPatrolHome;
    return 1;
}
int32_t CCellBase::NumberOfHVNeighbours() const {
    int32_t n = 0;
    for (uint8_t bit : {0x01, 0x04, 0x10, 0x40})
        if ((mNeighbours & bit) != 0) ++n;
    return n;
}
uint8_t CCellBase::GetNeighbours() const {
    gWalk.cells.push_back(IdOfCell(this));
    return mNeighbours;
}
uint8_t CCellBase::GetDirection() const { return mDirection; }
int32_t CMapCell::IsPath() const { return static_cast<int32_t>(mType) == 1 ? 1 : 0; }
int32_t CMapCell::IsQueue() const { return static_cast<int32_t>(mType) == 3 || static_cast<int32_t>(mType) == 9 ? 1 : 0; }
int32_t CMapCell::IsEntrance() const { return static_cast<int32_t>(mType) == 9 ? 1 : 0; }
int32_t CMapCell::IsExit() const { return static_cast<int32_t>(mType) == 0xa ? 1 : 0; }
int32_t CMapCell::IsWalkable() const { return static_cast<int32_t>(mType) == 0x1e ? 1 : 0; }
int32_t CMap::IsXYValid(int32_t x, int32_t y) { return x >= 0 && y >= 0 && x < 0x80 && y < 0x80; }
void CTRACE(uint32_t level, const char* format, ...) { gWalk.other.push_back(std::string("CTRACE ") + format); }
void GAME_ASSERTMSG(int32_t condition, const char* format, ...) {
    if (condition == 0) gWalk.other.push_back(std::string("ASSERT ") + format);
}

namespace {

int gFailures = 0;

void Check(bool ok, const std::string& what) {
    std::printf("%s %s\n", ok ? "ok  " : "FAIL", what.c_str());
    if (!ok) ++gFailures;
}

// The park. Path: a column at x 45 (y 41 to 64), the west bank's row at y 57 (x 38 to 50), the east bank's row at y 57
// (x 55 to 66) and a column at x 60 (y 50 to 64), and a row at y 41 (x 40 to 50) with the pavement (mode 0x1e, flag 0x80)
// beside it at y 40. The stream (water) at y 56 and 58, x 51 to 54, and with `bridge` the footbridge between, at y 57
// (mode 0x1e as CMapCell::Reset leaves it, flag 0x40); without it, water there too. Grass elsewhere.
const int kBridgeX0 = 51;
const int kBridgeX1 = 54;
const int kBridgeY = 57;

bool IsBridge(uint16_t id) { return YOf(id) == kBridgeY && XOf(id) >= kBridgeX0 && XOf(id) <= kBridgeX1; }

void Draw(bool bridge) {
    std::memset(static_cast<void*>(Cell(1)), 0, 128 * 128 * 0x44);
    for (int y = 0; y < 128; ++y)
        for (int x = 0; x < 128; ++x) {
            CMapCell* cell = Cell(IdOf(x, y));
            cell->mCellId = IdOf(x, y);
            cell->mKind = 1;
            cell->mFlags = 0x40;
        }
    auto set = [](int x, int y, int mode, uint16_t flags) {
        CMapCell* cell = Cell(IdOf(x, y));
        cell->mType = static_cast<CELL_MODES>(mode);
        cell->mFlags = flags;
    };
    for (int y = 41; y <= 64; ++y) set(45, y, 1, 0x20);
    for (int x = 38; x <= 50; ++x) set(x, 57, 1, 0x20);
    for (int x = 55; x <= 66; ++x) set(x, 57, 1, 0x20);
    for (int y = 50; y <= 64; ++y) set(60, y, 1, 0x20);
    for (int x = 40; x <= 50; ++x) set(x, 41, 1, 0x20);
    for (int x = 40; x <= 50; ++x) set(x, 40, 0x1e, 0xc0);
    for (int x = kBridgeX0; x <= kBridgeX1; ++x) {
        set(x, kBridgeY - 1, 2, 0x40);
        set(x, kBridgeY + 1, 2, 0x40);
        set(x, kBridgeY, bridge ? 0x1e : 2, 0x40);
    }
    // The path links, as CMapCell::CalculateNeighbours makes them: toward each path cell beside a path cell.
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {1, 0, -1, 0};
    const uint8_t bit[4] = {0x10, 0x04, 0x01, 0x40};
    for (int y = 1; y < 127; ++y)
        for (int x = 1; x < 127; ++x) {
            CMapCell* cell = Cell(IdOf(x, y));
            if (cell->mType != 1) continue;
            for (int i = 0; i < 4; ++i)
                if (Cell(IdOf(x + dx[i], y + dy[i]))->mType == 1) cell->mNeighbours = static_cast<uint8_t>(cell->mNeighbours | bit[i]);
        }
}

alignas(16) uint8_t gPeep[0x400];

void Place(uint16_t id) {
    gPeep[4] = static_cast<uint8_t>(XOf(id));
    gPeep[6] = static_cast<uint8_t>(YOf(id));
}

// `count` walks of a peep of `type` from `start`, each from where the last one ended, with the mod on or off.
std::vector<Walk> Wander(bool modOn, uint8_t type, uint16_t start, int count, uint32_t seed) {
    tpw::mods::Hooks hooks;
    if (modOn) tpw::mods::SwitchOn_staff_cross_bridges(hooks);
    tpw::mods::Apply(hooks);
    std::memset(gPeep, 0, sizeof gPeep);
    gPeep[2] = type;
    Place(start);
    gSeed = seed;
    std::vector<Walk> walks;
    for (int i = 0; i < count; ++i) {
        gWalk = Walk();
        gWalk.cells.push_back(IdOf(gPeep[4], gPeep[6]));   // where it stands
        gWalk.result = reinterpret_cast<CPeep*>(gPeep)->SetRandomDestination();
        walks.push_back(gWalk);
        if (gWalk.target != 0) Place(gWalk.target);
    }
    tpw::mods::Apply(tpw::mods::Hooks());
    return walks;
}

// Each walk steps only to the cell beside the last (or stays), and only on path or bridge cells, never on water,
// grass or the pavement; and takes one to five steps (one more when it comes back to its start, the original's rule).
bool Sound(const std::vector<Walk>& walks, std::string& why) {
    for (const Walk& w : walks) {
        std::vector<uint16_t> route;
        route.push_back(w.cells[0]);
        for (size_t i = 2; i < w.cells.size(); ++i) route.push_back(w.cells[i]);   // cells[1] is the start's own step
        // A walk FindDestInPatrolArea ends goes to its target by SetDest, not by steps.
        const bool home = !w.other.empty() && w.other.back() == "FindDestInPatrolArea";
        if (w.target != 0 && !home) route.push_back(w.target);
        if (w.cells.size() > 8) {
            why = "a walk of " + std::to_string(w.cells.size() - 1) + " steps";
            return false;
        }
        for (size_t i = 0; i < route.size(); ++i) {
            const CMapCell* cell = Cell(route[i]);
            const bool path = cell->mType == 1;
            const bool bridge = cell->mType == 0x1e && (cell->mFlags & 0x80) == 0;
            if (!path && !bridge) {
                why = "a walk onto (" + std::to_string(XOf(route[i])) + ", " + std::to_string(YOf(route[i])) + "), mode " +
                      std::to_string(static_cast<int>(cell->mType));
                return false;
            }
            if (i > 0) {
                const int d = std::abs(XOf(route[i]) - XOf(route[i - 1])) + std::abs(YOf(route[i]) - YOf(route[i - 1]));
                if (d > 1) {
                    why = "a jump from cell " + std::to_string(route[i - 1]) + " to " + std::to_string(route[i]);
                    return false;
                }
            }
        }
    }
    return true;
}

struct Seen {
    bool bridge = false;   // stood on or stepped through a bridge cell
    bool east = false;     // reached the east bank (x 55 or more)
    bool westAgain = false;   // back on the west bank (x 50 or less) after the east one
    bool beside = false;   // stood on the bank next to the bridge
    int maxX = 0;
    int crossings = 0;     // walks that took it from one bank to the other
};

Seen Look(const std::vector<Walk>& walks) {
    Seen s;
    int side = 0;   // -1 west, 1 east
    for (const Walk& w : walks) {
        std::vector<uint16_t> route = w.cells;
        if (w.target != 0) route.push_back(w.target);
        for (uint16_t id : route) {
            if (IsBridge(id)) s.bridge = true;
            if (id == IdOf(kBridgeX0 - 1, kBridgeY)) s.beside = true;
            if (XOf(id) > s.maxX && YOf(id) == kBridgeY) s.maxX = XOf(id);
            if (XOf(id) >= 55) {
                s.east = true;
                if (side == -1) ++s.crossings;
                side = 1;
            }
            if (XOf(id) <= 50) {
                if (s.east) s.westAgain = true;
                if (side == 1) ++s.crossings;
                side = -1;
            }
        }
    }
    return s;
}

bool SameWalks(const std::vector<Walk>& a, const std::vector<Walk>& b, std::string& why) {
    if (a.size() != b.size()) {
        why = "a different number of walks";
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].cells != b[i].cells || a[i].target != b[i].target || a[i].result != b[i].result) {
            why = "walk " + std::to_string(i) + " went elsewhere";
            return false;
        }
        if (a[i].draws != b[i].draws) {
            why = "walk " + std::to_string(i) + " drew other random numbers (" + std::to_string(a[i].draws.size()) + " and " +
                  std::to_string(b[i].draws.size()) + ")";
            return false;
        }
        if (a[i].other != b[i].other) {
            why = "walk " + std::to_string(i) + " made other calls";
            return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    // The world and its map grid: the walk finds exits at the world's 0x2d8 (gWorld->mMap), the cells at gWorldMap.
    const size_t bytes = 0x2d8 + 128 * 128 * 0x44 + 0x44;
    uint8_t* world = static_cast<uint8_t*>(std::calloc(1, bytes));
    gWorld = reinterpret_cast<CWorld*>(world);
    gWorldMap = reinterpret_cast<CMap*>(world + 0x2d8);

    const uint16_t west = IdOf(45, 57);
    const uint16_t bank = IdOf(kBridgeX0 - 1, kBridgeY);
    const int kWalks = 4000;
    std::string why;

    Draw(true);
    gPatrolHome = west;

    // A cleaner (type 4, the code's handyman) with no patrol area.
    for (uint32_t seed : {1u, 77u, 4242u}) {
        const std::string at = " (seed " + std::to_string(seed) + ")";
        const std::vector<Walk> off = Wander(false, 4, west, kWalks, seed);
        const Seen so = Look(off);
        Check(Sound(off, why), "mod off: every step is along the paths" + at + (why.empty() ? "" : ": " + why));
        Check(so.beside && !so.bridge && !so.east, "mod off: the cleaner reaches the bridge's end but never steps onto it" + at);

        const std::vector<Walk> on = Wander(true, 4, west, kWalks, seed);
        const Seen sn = Look(on);
        why.clear();
        Check(Sound(on, why), "mod on: every step is along a path or the bridge" + at + (why.empty() ? "" : ": " + why));
        Check(sn.bridge && sn.east && sn.westAgain && sn.crossings >= 2,
              "mod on: the cleaner steps onto the bridge, crosses it and steps off, and comes back (" +
                  std::to_string(sn.crossings) + " crossings)" + at);
    }

    // Every staff job (the peep types 4 to 8).
    for (uint8_t type = 4; type <= 8; ++type) {
        const Seen sn = Look(Wander(true, type, west, kWalks, 9));
        const Seen so = Look(Wander(false, type, west, kWalks, 9));
        Check(sn.east && !so.bridge, "staff type " + std::to_string(type) + ": crosses with the mod on, never with it off");
    }

    // A walk that starts on the bridge: off, the original's search around it (SetDest by cell id, to the nearest path);
    // on, a step along the bridge or onto the bank.
    {
        const std::vector<Walk> off = Wander(false, 4, IdOf(52, kBridgeY), 1, 5);
        Check(!off[0].other.empty() && off[0].other[0].rfind("SetDest ", 0) == 0, "mod off: a cleaner on the bridge looks around it for a path");
        const std::vector<Walk> on = Wander(true, 4, IdOf(52, kBridgeY), 1, 5);
        const uint16_t t = on[0].target;
        Check(on[0].other.empty() && (IsBridge(t) || Cell(t)->mType == 1) && on[0].result == 1,
              "mod on: a cleaner on the bridge walks along it or off it");
    }

    // The patrol area. Only the west bank: the bridge is never taken. Up to x 52: the bridge's first two cells are
    // taken, its last two and the far bank never.
    {
        gPatrol[0] = 30; gPatrol[1] = 30; gPatrol[2] = 50; gPatrol[3] = 70;
        const std::vector<Walk> west1 = Wander(true, 4, west, kWalks, 31);
        const Seen s1 = Look(west1);
        Check(s1.beside && !s1.bridge && !s1.east, "mod on: a bridge outside the patrol area is never taken");
        gPatrol[2] = 52;
        const std::vector<Walk> west2 = Wander(true, 4, west, kWalks, 31);
        const Seen s2 = Look(west2);
        Check(s2.bridge && s2.maxX == 52 && !s2.east, "mod on: the bridge is walked up to the patrol area's edge and no further");
        why.clear();
        Check(Sound(west2, why), "mod on: the patrol walks are along the paths and the bridge" + (why.empty() ? "" : ": " + why));
        // The same patrol from the east: it never crosses west.
        gPatrol[0] = 55; gPatrol[2] = 70;
        gPatrolHome = IdOf(60, 57);
        const Seen s3 = Look(Wander(true, 4, IdOf(60, 57), kWalks, 32));
        Check(!s3.bridge && s3.maxX >= 55, "mod on: from the east bank, a patrol area without the bridge keeps to it");
        gPatrol[0] = 0; gPatrol[1] = 0; gPatrol[2] = 127; gPatrol[3] = 127;
        gPatrolHome = west;
    }

    // Visitors (any type but 4 to 8) never take the new step: their walks and draws are the same with the mod on and off.
    for (uint8_t type : {0, 1, 2, 3, 9}) {
        const std::vector<Walk> off = Wander(false, type, west, kWalks, 11);
        const std::vector<Walk> on = Wander(true, type, west, kWalks, 11);
        why.clear();
        Check(SameWalks(off, on, why) && !Look(on).bridge,
              "visitor type " + std::to_string(type) + ": the same walks and draws with the mod on" + (why.empty() ? "" : ": " + why));
    }

    // A park with no bridge (the stream all water, the pavement still beside its path): every walk and every random draw
    // the same with the mod on and off, for every staff job, with and without a patrol area, from several places.
    Draw(false);
    for (uint8_t type = 4; type <= 8; ++type)
        for (uint16_t start : {west, bank, IdOf(45, 41), IdOf(60, 57)})
            for (int patrol = 0; patrol < 2; ++patrol) {
                if (patrol == 1) {
                    gPatrol[0] = 38; gPatrol[1] = 41; gPatrol[2] = 50; gPatrol[3] = 60;
                } else {
                    gPatrol[0] = 0; gPatrol[1] = 0; gPatrol[2] = 127; gPatrol[3] = 127;
                }
                const std::vector<Walk> off = Wander(false, type, start, 1500, 1000u + type + start);
                const std::vector<Walk> on = Wander(true, type, start, 1500, 1000u + type + start);
                size_t draws = 0;
                for (const Walk& w : on) draws += w.draws.size();
                why.clear();
                Check(SameWalks(off, on, why), "no bridge: staff type " + std::to_string(type) + " from (" + std::to_string(XOf(start)) +
                                                    ", " + std::to_string(YOf(start)) + ")" + (patrol ? " in a patrol area" : "") +
                                                    ", the same walks and " + std::to_string(draws) + " draws" +
                                                    (why.empty() ? "" : ": " + why));
            }
    gPatrol[0] = 0; gPatrol[1] = 0; gPatrol[2] = 127; gPatrol[3] = 127;

    std::free(world);
    std::printf("%d failure(s)\n", gFailures);
    return gFailures == 0 ? 0 : 1;
}
