// The tests of Better land buying's land_unbought hook: the port's LLCC_ReviseHoarding with the fence walk, the lock and the
// fill it calls (LLCC_BuildHoardingDB, LLCC_LockHoarding, LLCC_FillInsideOfPark and the hook's steps, src/game/park/
// LLCellSystem.cpp and LLCellRide.cpp), with the mod off and on, in a park drawn into a real map grid. The map and track
// cells lie where the game keeps them; the fence pieces' links are their hoarding masks. The cell functions the walk and the
// fill call are fakes that do what the game's do to the fields these tests read: links, types, parents, directions and
// flags (the fence's own flag 0x40 halves, mode 0x81, and the meshes are left out). LLCM_LeftMouseButtonUp, which the purchase
// clicks round its rectangle, draws each line as the game's mode 0x19 does: a fence piece on every block of it, each linked
// to the next.
//
// The park: the land from (22, 22) to (61, 61) is the park's, the gate between posts 31 and 39 on row 21, except a pocket
// from (42, 42) to (53, 53) and a strip, the cells x 46 and 47 from row 54 down, joining it to the land outside the south
// fence: every block that holds the park's land and land outside is a fence piece, linked to the next piece along the line
// between the two, and the fence filled once as a new park's is. The purchase fills the strip, LLCC_ReviseHoarding(45, 53, 47, 61): the land inside its fence line is
// the strip's. Mod off, the fill makes the pocket the park's too and deletes its fence, as the Mac does. Mod on, the pocket
// stays unowned with its fence closed across the strip's mouth and no links to the strip's deleted pieces. Then, mod on,
// buying the pocket makes it the park's and takes its fence down; and a purchase that closes nothing in leaves the park as
// with the mod off.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "mods.h"
#include "CellBase.h"
#include "LLCellSystem.h"
#include "MapCell.h"
#include "ObjectParts.h"
#include "ThingReceivers.h"
#include "TrackCell.h"
#include "ui_main.h"
#include "World.h"

namespace tpw::mods {
void SwitchOn_better_land_buying(Hooks& hooks);
}

// The globals of other files the purchase reads.
CWorld* gGameWorld;
extern const MapDelta gMapDeltaPlusX = {1, 0, 0};
extern const MapDelta gMapDeltaPlusY = {0, 1, 0};
extern const MapDelta gMapDeltaPlusXPlusY = {1, 1, 0};

namespace {

constexpr int kSide = 128;
alignas(16) uint8_t gWorldBlock[0x2d8 + 0x110000 + kSide * kSide * 0x28];
int32_t gMode = 0;                    // LLCC_SystemSetMode's
int32_t gClickX = -1, gClickY = -1;   // the last click of the drawing

uint16_t IdOf(int x, int y) { return static_cast<uint16_t>(1 + x + y * kSide); }
int XOf(uint16_t id) { return (id - 1) & 0x7f; }
int YOf(uint16_t id) { return (id - 1) >> 7; }
CMapCell* Map(int x, int y) { return reinterpret_cast<CMapCell*>(gWorldBlock + 0x2d8 + (IdOf(x, y) - 1) * 0x44); }
CTrackCell* Track(int x, int y) { return reinterpret_cast<CTrackCell*>(gWorldBlock + 0x1102d8 + (IdOf(x, y) - 1) * 0x28); }
bool OnMap(int x, int y) { return x >= 0 && y >= 0 && x < kSide && y < kSide; }

uint8_t Links(const CCellBase* cell) { return cell->mKind == 2 ? cell->mHoardingNeighbours : cell->mNeighbours; }

// A fence piece on the block at (x, y): mode 0x19, its three other cells mode 0xc naming it.
void Piece(int x, int y) {
    Track(x, y)->mType = static_cast<CELL_MODES>(0x19);
    for (const int d : {1, kSide, kSide + 1}) {
        CTrackCell* part = reinterpret_cast<CTrackCell*>(reinterpret_cast<uint8_t*>(Track(x, y)) + d * 0x28);
        part->mType = static_cast<CELL_MODES>(0xc);
        part->mParentID = IdOf(x, y);
    }
}

void Link(int x0, int y0, int x1, int y1) {
    const uint8_t a = x1 > x0 ? 4 : x1 < x0 ? 0x40 : y1 > y0 ? 0x10 : 1;
    Track(x0, y0)->mHoardingNeighbours |= a;
    Track(x1, y1)->mHoardingNeighbours |= OPPOSITE(a);
}

}  // namespace

// ---- the fakes
int32_t CMap::IsXYValid(int32_t x, int32_t y) { return OnMap(x, y) ? 1 : 0; }
uint8_t OPPOSITE(uint8_t d) {
    if (d == 0) return 0;
    if (d < 0x10) return static_cast<uint8_t>((d << 4) & 0xf0);
    return static_cast<uint8_t>(d >> 4);
}
uint8_t ADD_DIRECTION(uint8_t a, uint8_t b) {
    int n = 0;
    for (int first = a; static_cast<uint8_t>(first) > 1; first >>= 1) ++n;
    const int v = static_cast<int>(b) << n;
    return static_cast<uint8_t>(v > 0x80 ? (v >> 8) & 0xff : v & 0xff);
}
uint8_t CCellBase::GetNeighbours() const { return gLLCC4818 != 0 && mKind == 2 ? mHoardingNeighbours : mNeighbours; }
void CCellBase::RemoveNeighbour(uint8_t mask) {
    if (gLLCC4818 != 0 && mKind == 2) mHoardingNeighbours &= static_cast<uint8_t>(~mask);
    else mNeighbours &= static_cast<uint8_t>(~mask);
}
int32_t CCellBase::NumberOfNeighbours() const {
    int32_t n = 0;
    for (uint8_t bits = GetNeighbours(); bits != 0; bits &= static_cast<uint8_t>(bits - 1)) ++n;
    return n;
}
int32_t CCellBase::NumberOfHVNeighbours() const {
    const uint8_t bits = GetNeighbours();
    return ((bits & 1) != 0) + ((bits & 4) != 0) + ((bits & 0x10) != 0) + ((bits & 0x40) != 0);
}
void CCellBase::ModifyDirection(uint8_t direction) { mDirection = direction; }
uint8_t CCellBase::GetDirection() const { return mDirection; }
void CCellBase::SetOverlapCounter(int32_t count) { mOverlapCounter = static_cast<int16_t>(count); }
CMapCell* CCellBase::GetMapCellEquiv() { return reinterpret_cast<CMapCell*>(gWorldBlock + 0x2d8 + (mCellId - 1) * 0x44); }
const CMapCell* CCellBase::GetMapCellEquiv() const { return reinterpret_cast<const CMapCell*>(gWorldBlock + 0x2d8 + (mCellId - 1) * 0x44); }
int32_t CMapCell::IsFlag(uint16_t flag) const { return (mFlags & flag) != 0 ? 1 : 0; }
void CMapCell::SetFlag(uint16_t flag) { mFlags = flag; }
void CMapCell::UnSetFlag(uint16_t flag) { mFlags &= static_cast<uint16_t>(~flag); }
uint16_t CMapCell::GetFlag() { return mFlags; }
void CMapCell::SetHFieldTile() {}
CMapCell* CMapCell::GetNeighbouringCell(const MapDelta* delta, int32_t) {
    const int x = XOf(mCellId) + delta->mX, y = YOf(mCellId) + delta->mY;
    return OnMap(x, y) ? Map(x, y) : nullptr;
}
int32_t CTrackCell::IsHoarding() const { return static_cast<int32_t>(mType) == 0x19 ? 1 : 0; }
int32_t CTrackCell::IsTrackX() const { return static_cast<int32_t>(mType) == 0xc || static_cast<int32_t>(mType) == 0x11 ? 1 : 0; }
int32_t CTrackCell::IsFlag(uint16_t flag) const { return (mFlags & flag) != 0 ? 1 : 0; }
void CTrackCell::SetFlag(uint16_t flags) { mFlags = flags; }
void CTrackCell::UnSetFlag(uint16_t flags) { mFlags &= static_cast<uint16_t>(~flags); }
uint16_t CTrackCell::GetFlag() { return mFlags; }
uint8_t CTrackCell::IsCorner() const {
    const uint8_t n = GetNeighbours();
    if (NumberOfHVNeighbours() != 2) return 0;
    return (((n & 1) || (n & 0x10)) && ((n & 4) || (n & 0x40))) ? n : 0;
}
uint8_t CTrackCell::IsCrossRoad() const { return NumberOfHVNeighbours() > 2 ? GetNeighbours() : 0; }
CTrackCell* CTrackCell::GetNeighbouringCell(MapDelta delta, int32_t offset) {
    const int x = XOf(mCellId) + delta.mX * offset, y = YOf(mCellId) + delta.mY * offset;
    return OnMap(x, y) ? Track(x, y) : nullptr;
}
CTrackCell* CTrackCell::GetNeighbouringCell(uint8_t direction, int32_t offset) {
    const int dx = direction == 4 ? 1 : direction == 0x40 ? -1 : 0;
    const int dy = direction == 0x10 ? 1 : direction == 1 ? -1 : 0;
    const int x = XOf(mCellId) + dx * offset, y = YOf(mCellId) + dy * offset;
    return OnMap(x, y) ? Track(x, y) : nullptr;
}
void CTrackCell::Modify(CELL_MODES, int32_t, int32_t) {}
void CTrackCell::SetHFieldTile() {}
void CTrackCell::ModifyParentID(tag_IDX_TYPE<IDXTYPEUniqueNameMapId, uint16_t> id) { mParentID = id.mIndex; }
void CTrackCell::ModifyType(CELL_MODES mode) { mType = mode; }
// A deleted piece: the block's four track cells back to their start (the links of the pieces beside it are left).
void CTrackCell::QuickDelete() { Delete(0, 0, false); }
void CTrackCell::Delete(int32_t, int32_t, bool) {
    const int x = XOf(mCellId), y = YOf(mCellId);
    for (const int d : {0, 1, kSide, kSide + 1}) {
        CTrackCell* part = reinterpret_cast<CTrackCell*>(reinterpret_cast<uint8_t*>(Track(x, y)) + d * 0x28);
        part->mType = static_cast<CELL_MODES>(0);
        part->mParentID = 0;
        part->mHoardingNeighbours = 0;
        part->mNeighbours = 0;
    }
}
void LLCC_SystemSetMode(int32_t mode, bool) {
    gMode = mode;
    gClickX = -1;
}
// The clicks round the rectangle, in mode 0x19: a line of pieces from the last click to this one, each linked to the next.
void LLCM_LeftMouseButtonUp(int32_t x, int32_t y, uint16_t, int32_t) {
    if (gMode != 0x19) return;
    if (gClickX >= 0) {
        const int dx = x > gClickX ? 2 : x < gClickX ? -2 : 0;
        const int dy = y > gClickY ? 2 : y < gClickY ? -2 : 0;
        int cx = gClickX, cy = gClickY;
        Piece(cx, cy);
        while (cx != x || cy != y) {
            Piece(cx + dx, cy + dy);
            Link(cx, cy, cx + dx, cy + dy);
            cx += dx;
            cy += dy;
        }
    }
    gClickX = x;
    gClickY = y;
}
void UI_SetGameWindowCursor(int16_t) {}
void RecalculateParkDarkening() {}

namespace {

int gFailures = 0;

void Check(bool ok, const std::string& what) {
    std::printf("%s %s\n", ok ? "ok  " : "FAIL", what.c_str());
    if (!ok) ++gFailures;
}

bool gOwned[kSide][kSide];

bool Owned(int x, int y) { return OnMap(x, y) && gOwned[y][x]; }

// The park, its pieces on the blocks holding the park's land and land outside, linked along the line between them.
void Build(bool pocket) {
    std::memset(gOwned, 0, sizeof gOwned);
    for (int y = 22; y <= 61; ++y)
        for (int x = 22; x <= 61; ++x) gOwned[y][x] = true;
    for (int x = 33; x <= 38; ++x) gOwned[21][x] = true;   // the gate's cells (LLCC_DrawInitialHoarding)
    if (pocket) {
        for (int y = 42; y <= 53; ++y)
            for (int x = 42; x <= 53; ++x) gOwned[y][x] = false;
        for (int y = 54; y <= 61; ++y)
            for (int x = 46; x <= 47; ++x) gOwned[y][x] = false;
    }
    std::memset(gWorldBlock, 0, sizeof gWorldBlock);
    gGameWorld = reinterpret_cast<CWorld*>(gWorldBlock);
    for (int y = 0; y < kSide; ++y) {
        for (int x = 0; x < kSide; ++x) {
            Map(x, y)->mCellId = IdOf(x, y);
            Map(x, y)->mKind = 1;
            Map(x, y)->mFlags = gOwned[y][x] ? 0 : 0x40;
            Track(x, y)->mCellId = IdOf(x, y);
            Track(x, y)->mKind = 2;
        }
    }
    auto mixed = [](int x, int y) {
        const int owned = Owned(x, y) + Owned(x + 1, y) + Owned(x, y + 1) + Owned(x + 1, y + 1);
        return owned != 0 && owned != 4;
    };
    for (int y = 1; y + 1 < kSide; y += 2)
        for (int x = 1; x + 1 < kSide; x += 2)
            if (mixed(x, y)) Piece(x, y);
    for (int y = 1; y + 1 < kSide; y += 2) {
        for (int x = 1; x + 1 < kSide; x += 2) {
            if (!mixed(x, y)) continue;
            if (x + 2 < kSide && mixed(x + 2, y) &&
                (Owned(x + 1, y) != Owned(x + 1, y + 1) || Owned(x + 2, y) != Owned(x + 2, y + 1)))
                Link(x, y, x + 2, y);
            if (y + 2 < kSide && mixed(x, y + 2) &&
                (Owned(x, y + 1) != Owned(x + 1, y + 1) || Owned(x, y + 2) != Owned(x + 1, y + 2)))
                Link(x, y, x, y + 2);
        }
    }
    gLLCCWidth = kSide;
    gLLCCHeight = kSide;
    gHoardingLeft = 31;
    gHoardingTop = 21;
    gHoardingRight = 39;
    gHoardingBottom = 21;
    // The fill a new park's fence gets once (LLCC_DrawInitialHoarding: locked, filled, unlocked). Its scan up through the gate's
    // opening runs off the map and makes the column of land in front of the gate the park's, as in every park of the game; no
    // purchase is under way, so the hook takes no part.
    gLLCC4818 = 1;
    LLCC_LockHoarding(true);
    LLCC_FillInsideOfPark();
    LLCC_LockHoarding(false);
    gLLCC4818 = 0;
}

void SetMod(bool on) {
    tpw::mods::Hooks hooks;
    if (on) tpw::mods::SwitchOn_better_land_buying(hooks);
    tpw::mods::Apply(hooks);
}

bool Fence(int x, int y) { return Track(x, y)->mType == static_cast<CELL_MODES>(0x19); }
bool Unowned(int x, int y) { return Map(x, y)->IsFlag(0x40) != 0; }
int AllUnowned(int x0, int y0, int x1, int y1) {
    int n = 0;
    for (int y = y0; y <= y1; ++y)
        for (int x = x0; x <= x1; ++x) n += Unowned(x, y);
    return n;
}

// Every cell's fence piece (its links, or -1 for none) and land flags, to compare two parks; Same says where they first differ.
struct Cell {
    int piece;
    int flags;
    bool operator==(const Cell& other) const { return piece == other.piece && flags == other.flags; }
};
std::vector<Cell> Snapshot() {
    std::vector<Cell> out;
    for (int y = 0; y < kSide; ++y)
        for (int x = 0; x < kSide; ++x) out.push_back({Fence(x, y) ? Links(Track(x, y)) : -1, Map(x, y)->mFlags});
    return out;
}
bool Same(const std::vector<Cell>& a, const std::vector<Cell>& b) {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == b[i]) continue;
        std::printf("     (%d, %d): piece %d flags 0x%x, against piece %d flags 0x%x\n", static_cast<int>(i % kSide), static_cast<int>(i / kSide),
                    a[i].piece, a[i].flags, b[i].piece, b[i].flags);
        return false;
    }
    return true;
}

// Whether the pieces round the pocket (41 to 53 each way) are a closed loop: every piece on it has two links, both to pieces on
// it, and none to anything else.
bool PocketFenceClosed() {
    for (int y = 41; y <= 53; y += 2) {
        for (int x = 41; x <= 53; x += 2) {
            const bool edge = x == 41 || x == 53 || y == 41 || y == 53;
            if (Fence(x, y) != edge) return false;
            if (!edge) continue;
            const uint8_t links = Links(Track(x, y));
            int n = 0;
            for (const uint8_t d : {1, 4, 0x10, 0x40}) {
                if ((links & d) == 0) continue;
                ++n;
                const int nx = x + (d == 4 ? 2 : d == 0x40 ? -2 : 0), ny = y + (d == 0x10 ? 2 : d == 1 ? -2 : 0);
                if (nx < 41 || nx > 53 || ny < 41 || ny > 53 || !Fence(nx, ny)) return false;
                if ((Links(Track(nx, ny)) & OPPOSITE(d)) == 0) return false;
            }
            if (n != 2) return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    // Mod off: the strip filled, the pocket becomes the park's and its fence is deleted, as the Mac does.
    SetMod(false);
    Build(true);
    Check(Fence(45, 57) && Fence(47, 57) && Fence(45, 53) && Fence(53, 53) && AllUnowned(43, 43, 52, 52) == 100,
          "the park before: the strip's fence and the pocket's, and the pocket unowned");
    LLCC_ReviseHoarding(45, 53, 47, 61);
    Check(AllUnowned(43, 43, 52, 52) == 0, "mod off: the pocket becomes the park's");
    Check(!Fence(41, 41) && !Fence(53, 53) && !Fence(45, 53) && !Fence(47, 53), "mod off: the pocket's fence is deleted");
    Check(AllUnowned(46, 55, 47, 60) == 0 && !Fence(45, 57) && !Fence(47, 57), "mod off: the strip is the park's, its fence deleted");
    Check(Fence(45, 61) && Fence(47, 61) && Fence(21, 21) && Fence(61, 61) && Fence(31, 21) && Fence(39, 21),
          "mod off: the outer fence stands, closed across the strip's end");
    const std::vector<Cell> outerOff = Snapshot();

    // Mod on: the same purchase leaves the pocket unowned and fenced.
    SetMod(true);
    Build(true);
    LLCC_ReviseHoarding(45, 53, 47, 61);
    Check(AllUnowned(43, 43, 52, 52) == 100, "mod on: the pocket stays unowned");
    Check(AllUnowned(46, 55, 47, 60) == 0 && !Fence(45, 57) && !Fence(47, 57), "mod on: the strip is the park's, its fence deleted");
    gLLCC4818 = 1;
    Check(PocketFenceClosed(), "mod on: the pocket's fence is a closed loop, across the strip's old mouth");
    Check((Links(Track(45, 53)) & 0x10) == 0 && (Links(Track(47, 53)) & 0x10) == 0,
          "mod on: no link from the pocket's fence to the strip's deleted pieces");
    gLLCC4818 = 0;
    Check(Fence(45, 61) && Fence(47, 61) && Fence(21, 21) && Fence(61, 61) && Fence(31, 21) && Fence(39, 21),
          "mod on: the outer fence stands, closed across the strip's end");

    // Mod on, then the pocket bought: it becomes the park's and its fence comes down, as with the mod off.
    LLCC_ReviseHoarding(41, 41, 53, 53);
    Check(AllUnowned(42, 42, 53, 53) == 0, "mod on, the pocket bought: it is the park's");
    Check(!Fence(41, 41) && !Fence(53, 53) && !Fence(45, 53) && !Fence(47, 53), "mod on, the pocket bought: its fence is deleted");
    Check(Same(Snapshot(), outerOff), "mod on, the pocket bought: the park is the same as after the mod off's one purchase");

    // A purchase that closes nothing in is the same with the mod off and on.
    SetMod(false);
    Build(false);
    LLCC_ReviseHoarding(61, 31, 71, 41);
    const std::vector<Cell> plainOff = Snapshot();
    Check(AllUnowned(63, 33, 70, 40) == 0 && !Fence(61, 35) && Fence(71, 35), "mod off: an expansion east of the park");
    SetMod(true);
    Build(false);
    LLCC_ReviseHoarding(61, 31, 71, 41);
    Check(Same(Snapshot(), plainOff), "mod on: the same expansion leaves the same park");

    SetMod(false);
    std::printf("%s: %d failure(s)\n", gFailures == 0 ? "PASS" : "FAIL", gFailures);
    return gFailures == 0 ? 0 : 1;
}
