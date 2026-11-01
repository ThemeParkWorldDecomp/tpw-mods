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

The game has a whole strike path. Once a month the staff room (CStaffHQ::UpdateMonthly) looks at each staff type: one
on strike goes back to work, and one with a grievance first gets a warning to the player, then goes on strike the next
time it is looked at, and again after that while the grievance lasts. A grievance (CStaffHQ::StaffTypeHasAGrievance) is
a park more than a fifth littered, for the handymen, or more than three staff of a type who are on average too tired or
too unhappy. No strike comes before the park is 24 months old.

In the released game the staff room does this only in an open park with no visitors in it, which a park past 24 months
almost never is, so players only ever saw strikes with the strike cheat, which skips that test. With the mod on,
the staff room looks at the strikes in any open park, visitors or not. Nothing else changes: a closed park still has no
strikes, the warnings, strikes and their ends are the original's code, and the strike cheat works as before.

Saved games are unchanged: a strike is kept in a save as the original keeps one. A park saved during a strike and
played on with the mod off keeps that strike while it has visitors, because the staff room then looks at strikes only
as the original does.

## Tests

tests/test_enable_strikes.cpp runs the port's CStaffHQ::UpdateMonthly with the mod off and on, the functions it calls
replaced by fakes that record each call:

- Mod off: the cases of tpw-decomp's differential test for UpdateMonthly (tests/test_CStaffHQ_Serialise.py), each with
  the calls the Mac's program makes. A strike that is not forced is considered only in an open park with no visitors.
- Mod on: the same cases, an open park with visitors in it considering a strike, and a closed park still not.

It runs with the port's tests (`ctest -R mod_enable_strikes`).

Thanks to BMCTV and slappyhead for the strike reports.
