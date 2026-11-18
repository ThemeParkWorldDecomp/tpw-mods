# Entertainers' second acts

Eight entertainer costumes have a finished second act in the game's files that never plays: the vampire,
Frankenstein's monster and the hunchback in Halloween World, the dinosaur, the explorer and the tribal dancer in the
Lost Kingdom, and the orange alien and the purple monster in the Space Zone. With the mod on they perform it as well as
their first act, chosen at random each time an act starts. The act lasts as long and cheers visitors just as the first
does.

| | |
| --- | --- |
| Id | entertainer-second-acts |
| Version | 1.0.0 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Unchanged |
| Hooks | entertainer_second_acts |
| Needs | STP/TPW Launcher 0.6.0 or later |

## What it changes

An entertainer walks its rounds, and when visitors are near it stops and performs. In the original every costume has
one act. The game picks the act from a list in the costume's sprite file, and every file the game shipped lists one,
but eight of the files also hold a whole second act, drawn in all directions, that the list leaves out:

| World | Costume | Second act (with the mod) |
| --- | --- | --- |
| Halloween World | Vampire | Juggles wooden stakes |
| Halloween World | Frankenstein's monster | Juggles bolts and nuts |
| Halloween World | Hunchback | Bent over, swings a little golden bell round his body |
| Lost Kingdom | Dinosaur | Bounces a rock off its head |
| Lost Kingdom | Explorer | Twirls his rifle and spins it overhead |
| Lost Kingdom | Tribal dancer | Throws his spear up and spins a plate on its point |
| Space Zone | Orange alien | Glowing green balls circle it |
| Space Zone | Purple monster | Holds one pose, hunched down with its arms out |

With the mod on, each time one of these entertainers starts an act it is the first or the second, at random, about
half and half. Everything else is the original's:

- **Length.** An act lasts as long as the first act would (the entertainer's grade sets it), then the entertainer walks on.
- **Sound.** The acts have no sound of their own. The sound the game plays when an act ends is played for either act.
- **Visitors.** The crowd watches and cheers the same way, and their happiness goes up by the same amount.
- **Walks, breaks and strikes.** Unchanged: the mod changes only which picture an act shows.
- **The other costumes** (Wonder Land's three, spr_fl, spr_gn and spr_la, and the Space Zone's spr_sp) have no second act in their files and
  perform as before.

The purple monster's second act is not an animation: its file holds one picture for each direction. It is most likely
the monster's resting pose, filed in the wrong place (its file has no resting pose where the others keep theirs), so
with the mod on the monster sometimes holds that pose for a whole act.

The game's random numbers are not changed: the choice is made from the number the original already draws when an act
starts, so a park plays exactly as with the mod off apart from the pictures.

## Saved games

Saved games are unchanged: a park saved with the mod on loads and plays with it off. An entertainer saved in the middle
of a second act, loaded with the mod off, finishes that act (its sprite script is saved with the act it plays) and then
performs only its first act, as in the original.

## Details for modders

| What | Function | Mac 2.0.1 |
|---|---|---|
| The act's start, and the hook (Entertainer.cpp, TPW_MOD_HOOK entertainer_second_acts) | CEntertainer::SetState | 0x100fb934 |
| The act's end: the sound and the next state | CEntertainer::ModelState | 0x100fba44 |
| The sprite script's jump to a custom block (variable 12 the action, 14 its frames) | ESP_ForceGoto | 0x100da318 |
| The delayed jump the original makes at the next turn | CDelayedScript::ProcessSpriteScripts | 0x100facc0 |
| The world's random numbers | CWorld::GetRand | 0x1013b1e4 |

- **The acts.** An .ESP file has 16 actions (first record, records in a view, views) and four custom blocks of four
  bytes. A block's first byte picks the script that plays it (1: the loop at 0x1029fe58, every frame of the action in
  turn, for ever) and its second byte is the action plus one. Every shipped entertainer file has one block, (1, 5),
  action 4. The second acts are action 10 of spr_dr, spr_fk, spr_hu (hallow), spr_di, spr_ex, spr_na (jungle) and
  spr_sm (space), and action 11 of spr_mo (space). The vampire's block alone carries two more bytes, 2 and 40 (40 is the
  first record of its action 10); nothing reads them for an entertainer.
- **The original's pick.** SetState draws one random number r, then picks script 13 + r % (count - 1) among the
  file's count blocks, and 13 when there is one. The `count - 1` would never pick the last of several blocks; with one
  block it does not matter. The script table holds still-pose loops for actions 10 and 11 (0x1029fc10, 0x1029fc28)
  among 22 such loops, and script 6 (0x1029f460) plays action 11's first four frames once; nothing starts any of them,
  and none of them says how an act would be chosen. So the mod follows the pick SetState was written to make: one of the
  acts at random from r. It takes bit 16 of r, not bit 0, because the low bit of the game's generator only alternates
  from one draw to the next.
- **The hook.** With gModEntertainerSecondActs set (include/EntertainerSecondActs.h), a costume with one block and a
  second act in its file, and bit 16 of r set: after SetState asks for script 13 as the original does, the sprite
  script jumps to custom block 0 at once (ESP_ForceGoto, as ProcessSpriteScripts would at the next turn), the waiting
  script is cleared, and variables 12 and 14 are set to the second act's action and its frames. The waiting service
  interval is left for the next turn, as the original leaves it. Otherwise SetState is the original's, call for call.
- **Saved games.** The delayed script saves its waiting script (mNextAnim), and the script system saves each sprite
  script with its variables. A second act leaves no script waiting, so a park saved during one holds only values the
  original also saves: the script's position in the block's loop and variable 12, the act's action, which the loop
  plays until the act ends, with the mod on or off.

## Tests

tests/test_entertainer_second_acts.cpp runs the port's CEntertainer::SetState with the mod off and on, for sprite files
made in memory (no game data): the eight costumes with their acts' frames and names written as the loader gives them
and in other spellings, a costume without a second act, a vampire file without action 10, a file with two blocks of
its own and a costume with frames in action 10 that is not one of the eight. The random numbers come from a fixed
generator, and a small model of the sprite script follows each act to the action it plays after the next turn.

- Mod off: every act is action 4, started at the next turn, with one random number.
- Mod on: for each of the eight, 2000 acts: the second act exactly when bit 16 of the act's number is set, both acts
  many times, the same random numbers as with the mod off, a first act the original's call for call, and a second act
  started at once with nothing left waiting in the delayed script.
- Mod on, the other files and an entertainer without a sprite script: every act as with the mod off.
- Mod on, waiting, walking, walking to rest and resting: as with the mod off.
- The name matching, the choice of bit 16, and that the mod switches on its own hook and no other.

They run with the port's tests (`ctest -R mod_entertainer_second_acts`).
