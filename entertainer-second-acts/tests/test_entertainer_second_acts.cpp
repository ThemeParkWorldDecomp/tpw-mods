// The tests of Entertainers' second acts: the start of an entertainer's act (CEntertainer::SetState, the port's
// src/game/people/Entertainer.cpp) with the mod off and on, for each of the eight costumes with a second act, for a
// costume without one, and for the files the hook must leave alone. The functions it calls are fakes here: the random
// numbers come from a fixed generator that counts its draws, the delayed script's SetSpriteScript keeps the script
// waiting for the next turn as the game's does, and the sprite script is a small model of the game's: a goto to a
// custom block takes the block's action into variable 12 and that action's frames into variable 14, as ESP_ForceGoto
// does (src/game/Script.cpp). The next turn's ProcessSpriteScripts is played by the test, so each act is followed to
// the action its script then plays.
//
// Mod off: every act is the first (action 4), started at the next turn, with the original's calls and draws (tpw-decomp's
// tests/test_Entertainer.py compares SetState with the Mac's program). Mod on: an act of the eight costumes is the second
// act exactly when bit 16 of the act's random number is set, both acts come up, the draws are the same as with the mod
// off, and a second act leaves nothing waiting in the delayed script (a game saved then holds only the script's own
// variables). A costume without a second act, a file without the act's frames, a file with two acts and an entertainer
// without a script start every act as with the mod off, and the other states are the original's.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "mods.h"
#include "ESP_sprites.h"
#include "EntertainerSecondActs.h"
#include "HLSprite.h"
#include "ThingReceivers.h"
#include "World.h"
#include "analyser.h"

// The mod's own switch-on function (entertainer_second_acts.cpp; the launcher's catalog declares it too).
namespace tpw::mods {
void SwitchOn_entertainer_second_acts(Hooks& hooks);
}

// The globals of other files SetState reads.
CWorld* gWorld;
ESPSpriteGroup gESPGroups[kESPGroups];
ESP_CSprite* gESPSprites[kESPMaxSprites + 16];

namespace {

constexpr int32_t kEntertainers = 4;          // the entertainers' sprite set (gESPGroups[4])
constexpr int32_t kStateEntertaining = 0xe;
constexpr int32_t kFirstActScript = 0xd;
constexpr int32_t kHandle = 7;                // the entertainer's sprite script

// What one SetState did.
struct Act {
    std::vector<uint32_t> draws;       // the random numbers it drew
    std::vector<std::string> calls;    // the fakes it called, with their arguments
    int32_t nextAnim = 0;              // the delayed script's waiting script after it
    int32_t nextInterval = 0;          // the delayed script's waiting service interval after it
    int32_t action = -1;               // the action the sprite script plays after the next turn's ProcessSpriteScripts
    int32_t frames = -1;               // and its frames in a view (variable 14)
    int32_t state = -1;
    uint32_t started = 0;              // mTimeStartedEntertaining
};

Act gAct;
uint32_t gSeed = 1;

// The sprite script: variables 12 and 14 of the handle kHandle.
int32_t gVar12 = -1;
int32_t gVar14 = -1;
const ESP_CSprite* gScriptSprite = nullptr;   // the sprite the script draws (its variables 10 and 11)

int gFailures = 0;

void Check(bool ok, const std::string& what) {
    std::printf("%s: %s\n", ok ? "ok" : "FAILED", what.c_str());
    if (!ok) ++gFailures;
}

// A sprite of the entertainers' set: its name, its custom blocks' first bytes (the act's script and action + 1) and the
// frames of its actions 4, 10 and 11.
struct SpriteSpec {
    const char* name;
    int blocks;          // custom blocks with a non-zero first byte (mCustomCount)
    int frames4;
    int frames10;
    int frames11;
};

alignas(16) unsigned char gSpriteStorage[16][sizeof(ESP_CSprite)];

ESP_CSprite* MakeSprite(int slot, const SpriteSpec& spec) {
    std::memset(gSpriteStorage[slot], 0, sizeof(ESP_CSprite));
    ESP_CSprite* sprite = reinterpret_cast<ESP_CSprite*>(gSpriteStorage[slot]);
    std::snprintf(sprite->mName, sizeof(sprite->mName), "%s", spec.name);
    sprite->mCustomCount = static_cast<uint16_t>(spec.blocks);
    for (int b = 0; b < spec.blocks; ++b) {
        sprite->mCustom[b * 4] = 1;       // the script at gScriptStartAddresses[0]
        sprite->mCustom[b * 4 + 1] = 5;   // action 4
    }
    sprite->mActions[4].mFrames = static_cast<uint8_t>(spec.frames4);
    sprite->mActions[10].mFrames = static_cast<uint8_t>(spec.frames10);
    sprite->mActions[11].mFrames = static_cast<uint8_t>(spec.frames11);
    for (int a : {4, 10, 11}) sprite->mActions[a].mViews = sprite->mActions[a].mFrames != 0 ? 5 : 0;
    gESPSprites[slot] = sprite;
    return sprite;
}

}  // namespace

// ---- the fakes
uint32_t CWorld::GetRand() {
    gSeed ^= gSeed << 13;
    gSeed ^= gSeed >> 17;
    gSeed ^= gSeed << 5;
    gAct.draws.push_back(gSeed);
    return gSeed;
}
int32_t ESP_GetNumberOfCustomAnims(ESP_Sprites set, int32_t index) {
    gAct.calls.push_back("ESP_GetNumberOfCustomAnims " + std::to_string(static_cast<int32_t>(set)) + " " + std::to_string(index));
    return gESPSprites[gESPGroups[set].mFirst + index]->mCustomCount;
}
void CDelayedScript::SetSpriteScript(ScriptIdx script) {
    gAct.calls.push_back("SetSpriteScript " + std::to_string(static_cast<int32_t>(script)));
    mNextAnim = script;
}
void CStaff::SetState(StaffState state) { gAct.calls.push_back("CStaff::SetState " + std::to_string(static_cast<int32_t>(state))); }
void ESP_ForceGoto(int32_t handle, void* code) {
    const uintptr_t block = reinterpret_cast<uintptr_t>(code);
    gAct.calls.push_back("ESP_ForceGoto " + std::to_string(handle) + " " + std::to_string(block));
    if (handle != kHandle || block > 3) return;
    gVar12 = gScriptSprite->mCustom[block * 4 + 1] - 1;   // CustomStart: the block's action
    gVar14 = gScriptSprite->mActions[gVar12].mFrames;
}
void SCR_SetVariable(int32_t handle, uint32_t index, int32_t value) {
    gAct.calls.push_back("SCR_SetVariable " + std::to_string(handle) + " " + std::to_string(index) + " " + std::to_string(value));
    if (handle != kHandle) return;
    if (index == 12) gVar12 = value;
    if (index == 14) gVar14 = value;
}

namespace {

alignas(16) unsigned char gEntertainerStorage[sizeof(CEntertainer)];

// One SetState of the entertainer wearing sprite `slot`, then the next turn's ProcessSpriteScripts (script 13 is custom
// block 0, DelayedScript.cpp's table).
Act Start(bool modOn, int slot, int32_t state, int32_t handle = kHandle) {
    tpw::mods::Hooks hooks;
    if (modOn) tpw::mods::SwitchOn_entertainer_second_acts(hooks);
    tpw::mods::Apply(hooks);
    std::memset(gEntertainerStorage, 0, sizeof(gEntertainerStorage));
    CEntertainer* e = reinterpret_cast<CEntertainer*>(gEntertainerStorage);
    e->mSpriteSet = kEntertainers;
    e->mSpriteIndex = slot;
    CDelayedScript* script = e->Script();
    script->mSpriteScript = handle;
    script->mNextServiceInterval = 9;   // a walk's interval, still waiting
    gScriptSprite = gESPSprites[slot];
    gVar12 = 0;   // the walk's action
    gVar14 = 0;
    gAct = Act();
    e->SetState(static_cast<CStaff::StaffState>(state));
    gAct.nextAnim = static_cast<int32_t>(script->mNextAnim);
    gAct.nextInterval = script->mNextServiceInterval;
    gAct.state = e->mState;
    gAct.started = e->mTimeStartedEntertaining;
    if (gAct.nextAnim == kFirstActScript) ESP_ForceGoto(script->mSpriteScript, nullptr);
    gAct.action = gVar12;
    gAct.frames = gVar14;
    return gAct;
}

// Acts from the same seed with the mod off and on.
std::vector<Act> Acts(bool modOn, int slot, int count, uint32_t seed, int32_t state = kStateEntertaining, int32_t handle = kHandle) {
    gSeed = seed;
    std::vector<Act> acts;
    for (int i = 0; i < count; ++i) acts.push_back(Start(modOn, slot, state, handle));
    return acts;
}

bool SameActs(const std::vector<Act>& a, const std::vector<Act>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].draws != b[i].draws || a[i].calls != b[i].calls || a[i].nextAnim != b[i].nextAnim ||
            a[i].nextInterval != b[i].nextInterval || a[i].action != b[i].action || a[i].frames != b[i].frames ||
            a[i].state != b[i].state || a[i].started != b[i].started)
            return false;
    }
    return true;
}

}  // namespace

int main() {
    // The world: SetState reads only the turn (mTurn, 0x1da70c).
    uint8_t* world = static_cast<uint8_t*>(std::calloc(1, sizeof(CWorld)));
    gWorld = reinterpret_cast<CWorld*>(world);
    gWorld->mTurn = 1234;
    gESPGroups[kEntertainers].mFirst = 0;
    gESPGroups[kEntertainers].mCount = 12;

    // The eight costumes with a second act, as their files are (the frames of their acts), named as the loader names
    // them (the .ESP file's name without its extension) and in other spellings.
    const SpriteSpec kWithSecond[] = {
        {"hallow\\entertainers\\spr_dr", 1, 8, 8, 0}, {"spr_fk", 1, 8, 8, 0},   {"SPR_HU", 1, 16, 8, 0},
        {"jungle/entertainers/spr_di", 1, 8, 8, 0},   {"spr_ex", 1, 17, 8, 0},  {"Spr_Na", 1, 8, 8, 0},
        {"space:entertainers:spr_sm", 1, 8, 8, 0},    {"spr_mo", 1, 8, 0, 1},
    };
    const int32_t kSecondAction[] = {10, 10, 10, 10, 10, 10, 10, 11};
    for (int i = 0; i < 8; ++i) MakeSprite(i, kWithSecond[i]);
    MakeSprite(8, {"spr_fl", 1, 8, 0, 0});          // the fantasy world's fairy: no second act
    MakeSprite(9, {"spr_dr", 1, 8, 0, 0});          // a vampire file without action 10
    MakeSprite(10, {"spr_hu", 2, 16, 8, 0});        // a hunchback file with two acts of its own
    MakeSprite(11, {"spr_sp", 1, 8, 8, 0});         // a costume not of the eight, given action 10 frames

    // The helpers of include/EntertainerSecondActs.h.
    Check(EntertainerSecondActOfName("spr_dr") == 10 && EntertainerSecondActOfName("spr_mo") == 11,
          "the vampire's second act is action 10, the purple monster's action 11");
    Check(EntertainerSecondActOfName("HALLOW\\ENTERTAINERS\\SPR_FK") == 10 && EntertainerSecondActOfName("space/entertainers/spr_sm") == 10,
          "a name is matched by its last part, without case");
    Check(EntertainerSecondActOfName("spr_drx") == -1 && EntertainerSecondActOfName("xspr_dr") == -1 &&
              EntertainerSecondActOfName("spr_d") == -1 && EntertainerSecondActOfName("") == -1 &&
              EntertainerSecondActOfName(nullptr) == -1 && EntertainerSecondActOfName("spr_fl") == -1 &&
              EntertainerSecondActOfName("spr_sp") == -1 && EntertainerSecondActOfName("spr_dr/") == -1,
          "other names have no second act");
    Check(EntertainerSecondAct(*gESPSprites[9]) == -1, "a file without the act's frames has no second act");
    Check(EntertainerPlaysSecondAct(0x10000) && EntertainerPlaysSecondAct(0xffffffffu) && !EntertainerPlaysSecondAct(0xfffeffffu) &&
              !EntertainerPlaysSecondAct(1),
          "the second act is chosen by bit 16 of the act's random number");

    // The hook's switch: only the mod's own field, and Apply puts it into the game.
    {
        tpw::mods::Hooks hooks;
        tpw::mods::SwitchOn_entertainer_second_acts(hooks);
        tpw::mods::Hooks expected;
        expected.entertainerSecondActs = true;
        Check(std::memcmp(&hooks, &expected, sizeof(hooks)) == 0, "the mod switches on its hook and no other");
        tpw::mods::Apply(hooks);
        Check(gModEntertainerSecondActs, "Apply sets gModEntertainerSecondActs");
        tpw::mods::Apply(tpw::mods::Hooks());
        Check(!gModEntertainerSecondActs, "Apply of no hooks clears it");
    }

    const int kActs = 2000;

    // Mod off: every act is the first, waiting for the next turn, with one draw.
    for (int slot = 0; slot < 12; ++slot) {
        const std::vector<Act> off = Acts(false, slot, kActs, 100u + slot);
        bool first = true;
        for (const Act& a : off) {
            const int32_t script = gESPSprites[slot]->mCustomCount == 1 ? kFirstActScript : -1;
            if (a.draws.size() != 1 || a.state != kStateEntertaining || a.started != 1234 || a.nextInterval != 9) first = false;
            if (script > 0 && (a.nextAnim != script || a.action != 4 || a.frames != gESPSprites[slot]->mActions[4].mFrames)) first = false;
        }
        Check(first, std::string("mod off, ") + gESPSprites[slot]->mName + ": every act is action 4, started at the next turn, one draw");
    }

    // Mod on, the eight costumes: the second act exactly when bit 16 is set, both acts come up, the same draws.
    for (int slot = 0; slot < 8; ++slot) {
        const std::vector<Act> off = Acts(false, slot, kActs, 200u + slot);
        const std::vector<Act> on = Acts(true, slot, kActs, 200u + slot);
        int seconds = 0;
        int firsts = 0;
        bool rule = true;
        bool sameDraws = true;
        bool firstAsOff = true;
        bool secondNow = true;
        const ESP_CSprite* sprite = gESPSprites[slot];
        const int32_t second = kSecondAction[slot];
        for (size_t i = 0; i < on.size(); ++i) {
            const Act& a = on[i];
            if (a.draws != off[i].draws) sameDraws = false;
            const bool wantSecond = (a.draws.at(0) >> 16 & 1u) != 0;
            if (wantSecond) {
                ++seconds;
                if (a.action != second || a.frames != sprite->mActions[second].mFrames) rule = false;
                // Started at once: nothing waits in the delayed script, the walk's interval still does.
                const std::vector<std::string> calls = {
                    "ESP_GetNumberOfCustomAnims 4 " + std::to_string(slot), "SetSpriteScript 13", "ESP_ForceGoto 7 0",
                    "SCR_SetVariable 7 12 " + std::to_string(second),
                    "SCR_SetVariable 7 14 " + std::to_string(sprite->mActions[second].mFrames)};
                if (a.nextAnim != 0 || a.nextInterval != 9 || a.calls != calls || a.state != kStateEntertaining || a.started != 1234)
                    secondNow = false;
            } else {
                ++firsts;
                if (a.action != 4) rule = false;
                if (!SameActs({a}, {off[i]})) firstAsOff = false;
            }
        }
        const std::string name = sprite->mName;
        Check(rule, "mod on, " + name + ": action " + std::to_string(second) + " exactly when bit 16 of the draw is set, else action 4");
        Check(seconds > kActs / 3 && firsts > kActs / 3,
              "mod on, " + name + ": both acts come up (" + std::to_string(seconds) + " second, " + std::to_string(firsts) + " first)");
        Check(sameDraws, "mod on, " + name + ": the same random numbers as with the mod off");
        Check(firstAsOff, "mod on, " + name + ": a first act is the original's, call for call");
        Check(secondNow, "mod on, " + name + ": a second act starts at once and leaves nothing waiting in the delayed script");
    }

    // Mod on, the files the hook leaves alone: everything as with the mod off.
    for (int slot = 8; slot < 12; ++slot) {
        Check(SameActs(Acts(false, slot, kActs, 300u + slot), Acts(true, slot, kActs, 300u + slot)),
              std::string("mod on, ") + gESPSprites[slot]->mName + " (" +
                  (slot == 8 ? "no second act" : slot == 9 ? "no action 10 in its file" : slot == 10 ? "two acts in its file" : "not one of the eight") +
                  "): every act as with the mod off");
    }
    Check(SameActs(Acts(false, 0, kActs, 400u, kStateEntertaining, 0), Acts(true, 0, kActs, 400u, kStateEntertaining, 0)),
          "mod on, an entertainer without a sprite script: every act as with the mod off");

    // Mod on, the other states (waiting, walking, walking to rest, resting): the original's.
    for (int32_t state : {0, 1, 2, 3}) {
        Check(SameActs(Acts(false, 0, 200, 500u + state, state), Acts(true, 0, 200, 500u + state, state)),
              "mod on, state " + std::to_string(state) + ": as with the mod off");
    }

    tpw::mods::Apply(tpw::mods::Hooks());
    std::free(world);
    std::printf("%d failure(s)\n", gFailures);
    return gFailures == 0 ? 0 : 1;
}
