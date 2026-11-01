// The tests of Enable strikes: the staff room's monthly update (CStaffHQ::UpdateMonthly, the port's
// src/game/buildings/StaffHQ.cpp) with the mod off and on. The functions it calls are fakes here that record each call
// and answer from the case; the world is a zeroed block.
//
// Mod off: the cases of tpw-decomp's tests/test_CStaffHQ_Serialise.py for UpdateMonthly, which compare it with the Mac's
// program; here each one's calls are the Mac's, written out. A strike that is not forced (the staff room's 0x64 word
// zero) is considered only in an open park with no visitors in it.
// Mod on: an open park with visitors in it considers striking; a closed park still does not.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "mods.h"
#include "Debug.h"
#include "ThingReceivers.h"
#include "World.h"
#include "analyser.h"

// The mod's own switch-on function (enable_strikes.cpp; the launcher's catalog declares it too).
namespace tpw::mods {
void SwitchOn_enable_strikes(Hooks& hooks);
}

// The globals of other files the update reads.
CWorld* gWorld;

namespace {

std::vector<std::string> gCalls;   // what the fakes were asked, in order
int32_t gClosed = 0;               // what IsParkClosed answers
int32_t gVisitors = 0;             // what GetVisitorsInPark answers
int32_t gStriking = 0;             // what IsStriking answers
CParkAnalyser* const kAnalyser = reinterpret_cast<CParkAnalyser*>(static_cast<uintptr_t>(0x50000200));   // never read

void Call(const std::string& call) { gCalls.push_back(call); }

}  // namespace

void CTRACE(uint32_t level, const char* format, ...) { Call("CTRACE " + std::to_string(level)); }
void CTrainingCentre::UpdateMonthly() { Call("CTrainingCentre::UpdateMonthly"); }
int32_t CWorld::IsParkClosed() const {
    Call("IsParkClosed");
    return gClosed;
}
CParkAnalyser* CWorld::GetParkAnalyser() {
    Call("GetParkAnalyser");
    return kAnalyser;
}
int32_t CParkAnalyser::GetVisitorsInPark(uint32_t arg) const {
    Call("GetVisitorsInPark " + std::to_string(arg));
    return gVisitors;
}
int32_t CStaffHQ::IsStriking(StaffType type) {
    Call("IsStriking " + std::to_string(static_cast<int>(type)));
    return gStriking;
}
void CStaffHQ::StopStrikeInProgress(StaffType type) { Call("StopStrikeInProgress " + std::to_string(static_cast<int>(type))); }
void CStaffHQ::ConsiderStriking(StaffType type) { Call("ConsiderStriking " + std::to_string(static_cast<int>(type))); }

namespace {

int gFailures = 0;

struct Case {
    const char* what;
    uint32_t forced = 0;    // the staff room's 0x64 word: a strike forced (the strike cheat)
    int32_t closed = 0;
    int32_t visitors = 0;
    int only = -1;          // the one staff type with staff (its list's head is not its "no staff" id); -1: none
    bool sameTurn = false;  // that type was already looked at this turn
    int32_t striking = 0;
};

// The calls the update makes for `c`, with the strike gate as the Mac has it (`gateOn` false) or as the mod sets it.
std::vector<std::string> Expected(const Case& c, bool gateOn) {
    std::vector<std::string> calls = {"CTrainingCentre::UpdateMonthly"};
    if (c.forced == 0) {
        calls.push_back("IsParkClosed");
        bool stop = c.closed != 0;
        if (!stop && !gateOn) {
            calls.push_back("GetParkAnalyser");
            calls.push_back("GetVisitorsInPark 5");
            stop = c.visitors != 0;
        }
        if (stop) {
            calls.push_back("CTRACE 2");
            return calls;
        }
    }
    if (c.only >= 0 && !c.sameTurn) {
        calls.push_back("IsStriking " + std::to_string(c.only));
        calls.push_back((c.striking != 0 ? "StopStrikeInProgress " : "ConsiderStriking ") + std::to_string(c.only));
    }
    return calls;
}

void Run(const Case& c, bool modOn) {
    tpw::mods::Hooks hooks;
    if (modOn) {
        tpw::mods::SwitchOn_enable_strikes(hooks);
    }
    tpw::mods::Apply(hooks);

    static CWorld* world = static_cast<CWorld*>(std::calloc(1, sizeof(CWorld)));
    std::memset(static_cast<void*>(world), 0, sizeof(CWorld));
    gWorld = world;
    world->mTurn = 41;
    // Type t reads its list's head here (the Mac's order: handymen, mechanics, entertainers, guards, then the fifth).
    uint16_t* heads[5] = {&world->mF1DA73C, &world->mF1DA73E, &world->mF1DA740, &world->mF1DA744, &world->mF1DA742};
    for (int t = 0; t < 5; ++t) {
        gStaffHQNoStaff[t] = static_cast<uint16_t>(3 * t);
        *heads[t] = static_cast<uint16_t>(gStaffHQNoStaff[t] + (t == c.only ? 1 : 0));
    }
    alignas(CStaffHQ) static unsigned char block[sizeof(CStaffHQ)];
    std::memset(block, 0, sizeof block);
    CStaffHQ* hq = reinterpret_cast<CStaffHQ*>(block);
    hq->mF64 = c.forced;
    if (c.only >= 0) hq->mStrikes[c.only].mF08 = c.sameTurn ? world->mTurn : world->mTurn ^ 1;
    gClosed = c.closed;
    gVisitors = c.visitors;
    gStriking = c.striking;
    gCalls.clear();

    hq->UpdateMonthly();

    const std::vector<std::string> expected = Expected(c, modOn);
    bool ok = gCalls == expected;
    if (ok && c.only >= 0 && !(c.forced == 0 && (c.closed != 0 || (!modOn && c.visitors != 0))))
        ok = hq->mStrikes[c.only].mF08 == world->mTurn;   // looked at this turn
    std::printf("%s mod %s: %s\n", ok ? "ok  " : "FAIL", modOn ? "on " : "off", c.what);
    if (!ok) {
        ++gFailures;
        std::printf("      expected:");
        for (const std::string& call : expected) std::printf(" [%s]", call.c_str());
        std::printf("\n      made:    ");
        for (const std::string& call : gCalls) std::printf(" [%s]", call.c_str());
        std::printf("\n");
    }
}

}  // namespace

int main() {
    // test_CStaffHQ_Serialise.py's month_edges: the gate, then each staff type with the strike forced.
    std::vector<Case> cases;
    for (uint32_t forced : {0u, 1u})
        for (int32_t closed : {0, 1})
            for (int32_t visitors : {0, 3}) {
                Case c;
                c.what = "the gate";
                c.forced = forced;
                c.closed = closed;
                c.visitors = visitors;
                c.only = 3;
                cases.push_back(c);
            }
    for (int t = 0; t < 5; ++t)
        for (bool same : {false, true})
            for (int32_t striking : {0, 1}) {
                Case c;
                c.what = "one staff type, the strike forced";
                c.forced = 1;
                c.only = t;
                c.sameTurn = same;
                c.striking = striking;
                cases.push_back(c);
            }

    for (const Case& c : cases) Run(c, false);
    for (const Case& c : cases) Run(c, true);

    // What the mod is for, said once more: an open park with visitors in it considers a strike with the mod on, never
    // with it off, and a closed park never.
    Case open;
    open.what = "an open park with visitors";
    open.visitors = 250;
    open.only = 0;
    Run(open, false);
    Run(open, true);
    const bool considered = gCalls.back() == "ConsiderStriking 0";
    Case closed = open;
    closed.what = "a closed park with visitors";
    closed.closed = 1;
    Run(closed, true);
    const bool closedNone = gCalls.back() == "CTRACE 2";
    if (!considered || !closedNone) ++gFailures;

    // Off again: the hooks of a run with no mod on are the original's.
    Run(open, false);

    std::printf("%d failure(s)\n", gFailures);
    return gFailures == 0 ? 0 : 1;
}
