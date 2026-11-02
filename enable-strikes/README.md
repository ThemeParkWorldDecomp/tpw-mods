# Enable strikes

Staff can go on strike in an open park, as Bullfrog's code was written to do before the release blocked it. Strikes
still need a park older than 24 months and staff with a real grievance.

| | |
| --- | --- |
| Id | enable-strikes |
| Version | 1.0.0 |
| Author | TPW Decomp |
| Editions | US, European and Gold |
| Conflicts | None |
| Saved games | Unchanged |
| Hooks | strike_gate |

## What it changes

In the released game the staff room's monthly check (CStaffHQ::UpdateMonthly) looks at strikes only in an open park
with no visitors in it, which a park past 24 months almost never is, so players only ever saw strikes with the strike
cheat. With the mod on, the check runs in any open park, visitors or not. Nothing else changes: a closed park still
starts no strikes, the warnings, strikes and their ends are the original's code, and the strike cheat works as before.
The sections below say how that code behaves.

## How strikes work

Your staff can go on strike, but only once your park is at least **2 years old**. At the end of each month the
game checks each job (janitors, which the code calls handymen, mechanics, entertainers, guards, researchers) on its
own. A job has a grievance when:

- **They are worn out.** You have 4 or more staff in that job and their average energy is very low (under 15 out of 100).
  In practice this means they have no Staff Room they can walk to. Staff only rest when they are completely exhausted,
  and without a Staff Room they never recover.
- **They are miserable.** You have 4 or more staff in that job and their average happiness is under 15. Resting in a
  Staff Room is also what restores happiness, so this usually has the same cause.
- **Janitors only: the park is filthy.** More than a fifth of your paths are littered. This counts even with one janitor.

What happens next:

1. **First month with a grievance:** the advisor warns you that the group is "threatening to go on strike".
2. **Still a grievance next month:** they strike. They walk out of the park to the pavement by the road and stand
   there with placards. Their status reads "On strike". Staff of that job you hire during the strike join it.
3. **The strike lasts until the next month-end.** Then they go back to work for a month.
4. **If the problem is still there after that month,** they strike again, and keep doing so every other month until you fix it.

While a group is on strike you still pay their wages. Striking mechanics mean no repairs and no ride upgrades.
Striking janitors mean no cleaning, and so on. A group with 3 or fewer staff never strikes over tiredness or
unhappiness (janitors can still strike over litter).

## How to stop or prevent a strike

- **Build a Staff Room, and make sure staff can reach it by path.** In a big park, build several. This alone prevents
  almost every strike.
- **Keep the park clean.** Hire enough janitors and place bins so that less than a fifth of your paths are littered.
- **Train your staff.** A promotion cheers a worker right up, and trained staff tire more slowly and recover faster.
- **After a warning, fix the cause before the month ends.** If it is fixed in time, the advisor tells you they decided not to strike.
- **During a strike:** fix the cause and wait for the month-end, when they go back to work. To end it at once, close
  the park and wait until the last visitor has left. The strike ends as soon as the park is closed and empty.
- After a strike they are only back at work for a month. If the cause is still there at the next month-end, they strike again.

## Why the original blocked strikes

**Proven:** Bullfrog switched strikes off just before release. The manual still describes them, but an addendum on
the disc says: "We decided to remove the strike feature as it proved somewhat confusing & frustrating to players."
The code for strikes is all still in the game, as are the advisor's spoken lines in every language. A single extra test
in the monthly check lets strikes happen only when the park is open and has no visitors at all, which practically
never happens. The PC v2.0 patch and the Mac 2.0.1 release are identical here.

**Likely, judging from the code:** the feature was not finished. Two sets of advisor lines were recorded and translated
but never wired up: the "last warning" and the "strike is over" messages. As a result, you never heard that a strike had
ended. With the block in place, a strike that started in a busy park would never end on its own. A park without a
Staff Room sees almost every job walk out at the same time. There is no sign that strikes crashed the game.

This mod removes only the visitors test, so the monthly check runs whenever the park is open. Everything else is the
original's logic. The missing advisor messages stay missing.

## The debug key

In cheat mode, Ctrl+Alt+S (Windows, Linux) or Control+Option+S (Mac) forces strikes. It works with or without this mod.

- It turns on a "force strikes" switch for every job. It also runs an **extra month-end straight away**, so you pay a
  month of wages and training each time you press it.
- Nothing happens before the park is 2 years old. The switch stays on, though, and strikes start once you get there.
- The first press gives the warnings. The next month-end, or another press, starts the strikes.
- **A second press does not reliably turn the switch off** (a bug in the original). Whether it ends up on or off
  depends on which jobs are already striking. With no researchers it never turns off. If you press it again while
  the game is paused, nothing happens until the game moves on.
- The switch is saved with your park. To get rid of it, load a save from before you pressed the key.

## Details for modders

| What | Function | Mac 2.0.1 |
|---|---|---|
| Monthly check and the gate (StaffHQ.cpp:139) | CStaffHQ::UpdateMonthly | 0x1012ae40 |
| 24-month rule, escalation, advisor posts | CStaffHQ::ConsiderStriking | 0x1012b084 |
| Triggers (litter > 0.2; more than 3 staff with average energy or happiness < 15) | CStaffHQ::StaffTypeHasAGrievance | 0x1012b56c |
| Striking flag | CStaffHQ::IsStriking / StopStrikeInProgress | 0x1012bde0 / 0x1012be24 |
| Debug toggle | CStaffHQ::ForceStrike | 0x1012c04c |
| Debug key (ForceStrike x5, then CMsgEndOfMonth) | start_strike | 0x1014ceac |
| Walk to the picket line | CStaff::DoGeneralStaffThing | 0x101272b0 |
| Picketing, ends when the park is closed and empty | CStaff::ProcessStriking | 0x10126504 |
| Advisor message = situation x 5 + type base | CAdvisor::HandleStaffInfo | 0x1000c40c |

- **Situations:** 0 warning, 1 last warning (unused), 2 strike, 3 called off, 4 strike over (unused).
- **Text:** TAG_SYSTEM 38-62, in groups of five: mechanics, janitors, guards, entertainers, researchers.
- **Data:** Standard.sam holds RestLevel, RecuperationRate, HappinessRecuperationRate, HappyHitCosNoRestArea, and
  StrikeArea (40, 9, 6 x 1).

## Saved games

Saved games are unchanged: a strike is kept in a save as the original keeps one. A park saved during a strike and
played on with the mod off keeps that strike while it has visitors, because the monthly check then runs only as the
original's does. The same holds for the debug key without the mod: a strike it started in a busy park, with the switch
turned off again, never ends. With the mod on it ends at the next month-end.

## Tests

tests/test_enable_strikes.cpp runs the port's CStaffHQ::UpdateMonthly with the mod off and on, the functions it calls
replaced by fakes that record each call:

- Mod off: the cases of tpw-decomp's differential test for UpdateMonthly (tests/test_CStaffHQ_Serialise.py), each with
  the calls the Mac's program makes. A strike that is not forced is considered only in an open park with no visitors.
- Mod on: the same cases, an open park with visitors in it considering a strike, and a closed park still not.

It runs with the port's tests (`ctest -R mod_enable_strikes`).

Thanks to BMCTV and slappyhead for the strike reports.
