// The tests of Enable strikes' two advisor messages, which the original has but never posts: "strike over" when a strike
// ends (CStaffHQ::StopStrikeInProgress) and "last warning" seven days before the month's strike check
// (CStaffHQ::ModEndOfDay, which the park clock's new day calls, CParkTime::Update). These three are the port's
// src/game/buildings/StaffHQ.cpp and src/game/people/ParkTime.cpp; what they call is a fake here that records each call
// and answers from the case. The world, the staff room and the park clock are zeroed blocks.
//
// Strike over: posted once a strike, however many strikers ask for its end (a closed, empty park sends each of them), and
// never for a strike not in progress. Last warning: only on the seventh day before the next month, only to a staff type
// with staff that the month's check would send on strike (a warning given, or a strike over) and that still has its
// grievance, and only in an open park or with the strike forced. Mod off: neither, and the clock does not call the staff
// room at all.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "mods.h"
#include "CMessageCentre.h"
#include "CSubscriptionHeader.h"
#include "DateMessages.h"
#include "ParkTime.h"
#include "ThingReceivers.h"
#include "UIInterface.h"
#include "World.h"
#include "ui_main.h"

// The mod's own switch-on function (enable_strikes.cpp; the launcher's catalog declares it too).
namespace tpw::mods {
void SwitchOn_enable_strikes(Hooks& hooks);
}

// The globals of other files the three read.
CWorld* gWorld;
CMessageCentre* gMessageCentre;
UIInterface gUIInterface;

namespace {

std::vector<std::string> gCalls;   // what the fakes were asked, in order
int32_t gClosed = 0;               // what IsParkClosed answers
int32_t gGrievance[5];             // what StaffTypeHasAGrievance answers, by staff type
CStaffHQ* gHq = nullptr;           // what GetStaffHQ answers
int32_t gNow[3] = {2001, 1, 1};    // the funny time's year, month (1 to 12) and day, for GetFunnyTime

void Call(const std::string& call) { gCalls.push_back(call); }

}  // namespace

// ---- StaffHQ.cpp's callees
int32_t CWorld::IsParkClosed() const {
    Call("IsParkClosed");
    return gClosed;
}
int32_t CStaffHQ::StaffTypeHasAGrievance(StaffType type) {
    Call("StaffTypeHasAGrievance " + std::to_string(static_cast<int>(type)));
    return gGrievance[static_cast<int>(type)];
}
void CStaffHQ::ModPostStaffInfo(StaffType type, StaffSituation situation) {
    Call("post " + std::to_string(static_cast<int>(type)) + " " + std::to_string(static_cast<int>(situation)));
}

// ---- ParkTime.cpp's callees
TbTimeStamp CParkTime::GetFunnyTime(int32_t* year, int32_t* month, int32_t* day, int32_t* dayOfWeek, int32_t* hour, int32_t* minute,
                                    int32_t* second, int32_t* millisecond) const {
    if (year != nullptr) *year = gNow[0];
    if (month != nullptr) *month = gNow[1];
    if (day != nullptr) *day = gNow[2];
    for (int32_t* other : {dayOfWeek, hour, minute, second, millisecond})
        if (other != nullptr) *other = 0;
    return TbTimeStamp();
}
CSubscriptionHeader::CSubscriptionHeader() {}   // the message centre's, which the clock makes when there is none (never here)
CSubscriptionHeader::~CSubscriptionHeader() {}
int32_t CMessageCentre::PostMessage(const CMsg& message) {
    Call("PostMessage " + std::to_string(message.GetType()));
    return 1;
}
CMsgEndOfDay::CMsgEndOfDay() { mFlags = 0; }
CMsgEndOfDay::~CMsgEndOfDay() {}
int32_t CMsgEndOfDay::GetType() const { return 0xb; }
CMsgEndOfMonth::CMsgEndOfMonth() { mFlags = 0; }
CMsgEndOfMonth::~CMsgEndOfMonth() {}
int32_t CMsgEndOfMonth::GetType() const { return 0xc; }
CMsgEndOfYear::CMsgEndOfYear() { mFlags = 0; }
CMsgEndOfYear::~CMsgEndOfYear() {}
int32_t CMsgEndOfYear::GetType() const { return 0xd; }
void UIInterface::DateChange(ParkDate) {}
void UI_ParkStatusPanelDateChange(int32_t) {}
void UI_FinancePanelPanelDateChange(int32_t) {}
void UI_LoansPanelDateChange(int32_t) {}
CStaffHQ* CWorld::GetStaffHQ() {
    Call("GetStaffHQ");
    return gHq;
}

namespace {

int gFailures = 0;

void Check(bool ok, const std::string& what, const std::vector<std::string>& expected) {
    std::printf("%s %s\n", ok ? "ok  " : "FAIL", what.c_str());
    if (ok) return;
    ++gFailures;
    std::printf("      expected:");
    for (const std::string& call : expected) std::printf(" [%s]", call.c_str());
    std::printf("\n      made:    ");
    for (const std::string& call : gCalls) std::printf(" [%s]", call.c_str());
    std::printf("\n");
}

// Only the calls of `prefix`.
std::vector<std::string> Only(const std::string& prefix) {
    std::vector<std::string> kept;
    for (const std::string& call : gCalls)
        if (call.compare(0, prefix.size(), prefix) == 0) kept.push_back(call);
    return kept;
}

void SwitchMod(bool on) {
    tpw::mods::Hooks hooks;
    if (on) tpw::mods::SwitchOn_enable_strikes(hooks);
    tpw::mods::Apply(hooks);
}

CStaffHQ* FreshStaffRoom() {
    alignas(CStaffHQ) static unsigned char block[sizeof(CStaffHQ)];
    std::memset(block, 0, sizeof block);
    return reinterpret_cast<CStaffHQ*>(block);
}

// A world in which the staff types of `with` have staff (each list's head is not its "no staff" id).
void FreshWorld(uint32_t with) {
    static CWorld* world = static_cast<CWorld*>(std::calloc(1, sizeof(CWorld)));
    std::memset(static_cast<void*>(world), 0, sizeof(CWorld));
    gWorld = world;
    // Type t reads its list's head here (the Mac's order: handymen, mechanics, entertainers, guards, then the fifth).
    uint16_t* heads[5] = {&world->mF1DA73C, &world->mF1DA73E, &world->mF1DA740, &world->mF1DA744, &world->mF1DA742};
    for (int t = 0; t < 5; ++t) {
        gStaffHQNoStaff[t] = static_cast<uint16_t>(3 * t + 1);
        *heads[t] = static_cast<uint16_t>(gStaffHQNoStaff[t] + ((with >> t) & 1));
    }
}

int DaysIn(int year, int month) {   // month 1 to 12
    static const int kDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) return 29;
    return kDays[month - 1];
}

// ---- strike over
void StrikeOver(bool modOn) {
    SwitchMod(modOn);
    FreshWorld(0x1f);
    for (int t = 0; t < 5; ++t) {
        CStaffHQ* hq = FreshStaffRoom();
        const std::string type = std::to_string(t);
        const std::string mod = modOn ? "mod on: " : "mod off: ";

        // A strike in progress, ended by the month's check: one post.
        hq->mStrikes[t].mF00 = 2;
        hq->mStrikes[t].mF04 = 1;
        gCalls.clear();
        hq->StopStrikeInProgress(static_cast<StaffType>(t));
        std::vector<std::string> expected;
        if (modOn) expected.push_back("post " + type + " 4");
        Check(gCalls == expected && hq->mStrikes[t].mF04 == 0 && hq->mStrikes[t].mF00 == 2,
              mod + "type " + type + ", the month's check ends a strike", expected);

        // A strike in progress, ended by a closed, empty park: each of eight strikers asks, on each of three turns. One post.
        hq->mStrikes[t].mF00 = 3;
        hq->mStrikes[t].mF04 = 1;
        gCalls.clear();
        for (int turn = 0; turn < 3; ++turn)
            for (int striker = 0; striker < 8; ++striker) hq->StopStrikeInProgress(static_cast<StaffType>(t));
        Check(gCalls == expected && hq->mStrikes[t].mF04 == 0,
              mod + "type " + type + ", eight strikers in a closed, empty park end it on three turns", expected);

        // No strike in progress (ConsiderStriking ends one when the grievance is gone): nothing.
        gCalls.clear();
        hq->StopStrikeInProgress(static_cast<StaffType>(t));
        Check(gCalls.empty() && hq->mStrikes[t].mF04 == 0, mod + "type " + type + ", no strike in progress", {});

        // Another strike, another end: one post again; the other types' records are untouched.
        hq->mStrikes[t].mF04 = 1;
        gCalls.clear();
        hq->StopStrikeInProgress(static_cast<StaffType>(t));
        bool others = true;
        for (int u = 0; u < 5; ++u)
            if (u != t && (hq->mStrikes[u].mF00 != 0 || hq->mStrikes[u].mF04 != 0 || hq->mStrikes[u].mF08 != 0)) others = false;
        Check(gCalls == expected && others, mod + "type " + type + ", the next strike's end", expected);
    }
}

// ---- last warning: the staff room's daily look, on the warning's day
struct Warn {
    int type = 0;
    uint32_t level = 1;      // the strike level (mF00): 0 none, 1 warned, 2 to 4 struck before
    uint32_t striking = 0;   // a strike in progress (mF04)
    int32_t grievance = 1;
    int32_t closed = 0;
    uint32_t forced = 0;     // the strike cheat (the staff room's 0x64 word)
    bool staff = true;       // the type has staff
};

// The calls ModEndOfDay makes on the warning's day for `w` (the one type in the world that has a record).
std::vector<std::string> ExpectedWarn(const Warn& w) {
    std::vector<std::string> calls;
    if (w.forced == 0) {
        calls.push_back("IsParkClosed");
        if (w.closed != 0) return calls;
    }
    if (!w.staff) return calls;
    const bool next = w.level == 1 || (w.level >= 2 && w.level <= 4 && w.striking == 0);
    if (!next) return calls;
    const std::string type = std::to_string(w.type);
    calls.push_back("StaffTypeHasAGrievance " + type);
    if (w.grievance != 0) calls.push_back("post " + type + " 1");
    return calls;
}

void LastWarningCases() {
    SwitchMod(true);
    int run = 0;
    int posted = 0;
    for (int t = 0; t < 5; ++t)
        for (uint32_t level = 0; level <= 5; ++level)
            for (uint32_t striking : {0u, 1u})
                for (int32_t grievance : {0, 1})
                    for (int32_t closed : {0, 1})
                        for (uint32_t forced : {0u, 1u})
                            for (bool staff : {false, true}) {
                                Warn w;
                                w.type = t;
                                w.level = level;
                                w.striking = striking;
                                w.grievance = grievance;
                                w.closed = closed;
                                w.forced = forced;
                                w.staff = staff;
                                FreshWorld(staff ? 1u << t : 0u);
                                CStaffHQ* hq = FreshStaffRoom();
                                hq->mF64 = forced;
                                hq->mStrikes[t].mF00 = level;
                                hq->mStrikes[t].mF04 = striking;
                                hq->mStrikes[t].mF08 = 77;
                                gClosed = closed;
                                for (int u = 0; u < 5; ++u) gGrievance[u] = u == t ? grievance : 1;
                                gParkDate.mYear = 101;   // 2001
                                gParkDate.mMonth = 5;    // June: 30 days, the warning on the 24th
                                gParkDate.mDay = 24;
                                gCalls.clear();
                                hq->ModEndOfDay();
                                const std::vector<std::string> expected = ExpectedWarn(w);
                                const bool unchanged = hq->mStrikes[t].mF00 == level && hq->mStrikes[t].mF04 == striking &&
                                                       hq->mStrikes[t].mF08 == 77 && hq->mF64 == forced;
                                ++run;
                                if (gCalls != expected || !unchanged) {
                                    Check(false,
                                          "last warning: type " + std::to_string(t) + " level " + std::to_string(level) + " striking " +
                                              std::to_string(striking) + " grievance " + std::to_string(grievance) + " closed " +
                                              std::to_string(closed) + " forced " + std::to_string(forced) + " staff " +
                                              std::to_string(staff),
                                          expected);
                                }
                                if (!expected.empty() && expected.back().compare(0, 4, "post") == 0) ++posted;
                            }
    std::printf("ok   last warning: %d cases of type, level, strike, grievance, gate and staff (%d post)\n", run, posted);
}

// Every staff type at once: each type that would strike gets its own warning, in the types' order.
void LastWarningAllTypes() {
    SwitchMod(true);
    FreshWorld(0x1f);
    CStaffHQ* hq = FreshStaffRoom();
    const uint32_t levels[5] = {1, 0, 4, 2, 3};
    const uint32_t striking[5] = {0, 0, 0, 1, 0};
    for (int t = 0; t < 5; ++t) {
        hq->mStrikes[t].mF00 = levels[t];
        hq->mStrikes[t].mF04 = striking[t];
        gGrievance[t] = t == 4 ? 0 : 1;
    }
    gClosed = 0;
    gParkDate.mYear = 101;
    gParkDate.mMonth = 0;   // January: 31 days, the warning on the 25th
    gParkDate.mDay = 25;
    gCalls.clear();
    hq->ModEndOfDay();
    const std::vector<std::string> expected = {"IsParkClosed", "StaffTypeHasAGrievance 0", "post 0 1", "StaffTypeHasAGrievance 2",
                                               "post 2 1", "StaffTypeHasAGrievance 4"};
    Check(gCalls == expected, "last warning: five types, two to warn (one warned, one struck before), one striking, one without a grievance",
          expected);
}

// ---- last warning: the park clock, day by day
// Walks the clock from the first of the month to the first of the next, one Update a day and a second Update on the same
// day, with one type warned and its grievance still there. Mod on: the clock asks the staff room on each new day and the
// one warning is posted on the seventh day before the next month. Mod off: the staff room is never asked.
void ClockWalk(bool modOn, int year, int month) {
    SwitchMod(modOn);
    FreshWorld(1u << 1);
    CStaffHQ* hq = FreshStaffRoom();
    hq->mStrikes[1].mF00 = 1;
    gHq = hq;
    gClosed = 0;
    for (int u = 0; u < 5; ++u) gGrievance[u] = 1;
    alignas(CParkTime) static unsigned char block[sizeof(CParkTime)];
    std::memset(block, 0, sizeof block);
    CParkTime* clock = reinterpret_cast<CParkTime*>(block);
    clock->mDayAtLastUpdate = 0;
    clock->mMonthAtLastUpdate = month - 2;
    clock->mF18 = year;
    alignas(CMessageCentre) static unsigned char centre[sizeof(CMessageCentre)];
    gMessageCentre = reinterpret_cast<CMessageCentre*>(centre);

    const int days = DaysIn(year, month);
    std::vector<std::string> warnings;
    int asked = 0;
    int newDays = 0;
    std::string warnedOn;
    for (int day = 1; day <= days + 1; ++day) {
        const bool next = day > days;
        gNow[0] = year + (next && month == 12 ? 1 : 0);
        gNow[1] = next ? month % 12 + 1 : month;
        gNow[2] = next ? 1 : day;
        for (int again = 0; again < 2; ++again) {
            gCalls.clear();
            clock->Update();
            if (std::find(gCalls.begin(), gCalls.end(), "PostMessage 11") != gCalls.end()) ++newDays;
            const std::vector<std::string> askedNow = Only("GetStaffHQ");
            asked += static_cast<int>(askedNow.size());
            for (const std::string& post : Only("post ")) {
                warnings.push_back(post);
                warnedOn = std::to_string(gNow[1]) + "/" + std::to_string(gNow[2]);
            }
        }
    }
    const std::string when = std::to_string(year) + "-" + std::to_string(month) + " (" + std::to_string(days) + " days)";
    if (modOn) {
        const std::string day = std::to_string(month) + "/" + std::to_string(days - 6);
        Check(warnings == std::vector<std::string>{"post 1 1"} && warnedOn == day && asked == newDays && newDays == days + 1,
              "mod on: " + when + ", one last warning on " + day + ", the staff room asked on each new day", {"post 1 1"});
    } else {
        Check(warnings.empty() && asked == 0 && newDays == days + 1, "mod off: " + when + ", no warning, the staff room never asked", {});
    }
    gHq = nullptr;
}

}  // namespace

int main() {
    StrikeOver(false);
    StrikeOver(true);
    LastWarningCases();
    LastWarningAllTypes();
    // A 30 and a 31 day month, February in a leap year, in a common year and in a century year that is not a leap year,
    // and December (the next month in the next year).
    const int walks[][2] = {{2001, 6}, {2001, 7}, {2000, 2}, {2001, 2}, {2100, 2}, {2001, 12}};
    for (const auto& walk : walks) ClockWalk(false, walk[0], walk[1]);
    for (const auto& walk : walks) ClockWalk(true, walk[0], walk[1]);

    // Not the warning's day: nothing is looked at.
    SwitchMod(true);
    FreshWorld(0x1f);
    CStaffHQ* hq = FreshStaffRoom();
    for (int t = 0; t < 5; ++t) hq->mStrikes[t].mF00 = 1;
    gParkDate.mYear = 101;
    gParkDate.mMonth = 5;
    bool none = true;
    for (int day = 1; day <= 30; ++day) {
        if (day == 24) continue;
        gParkDate.mDay = day;
        gCalls.clear();
        hq->ModEndOfDay();
        if (!gCalls.empty()) none = false;
    }
    Check(none, "last warning: June's other 29 days look at nothing", {});

    // Off again: the hooks of a run with no mod on are the original's.
    SwitchMod(false);
    Check(!gModStrikeMessages && !gModStrikeGate, "mod off: both hooks off again", {});

    std::printf("%d failure(s)\n", gFailures);
    return gFailures == 0 ? 0 : 1;
}
