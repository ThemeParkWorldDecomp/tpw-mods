# tpw-mods

Optional changes to how Theme Park World plays, for the port in
[tpw-port](https://github.com/ThemeParkWorldDecomp/tpw-port). Each mod is off until the player turns it on, and with
every mod off the game is exactly the original.

## Mods

| Mod | What it does | Editions | Saved games |
| --- | --- | --- | --- |
| [Enable strikes](enable-strikes/README.md) | Staff can go on strike in an open park, as Bullfrog's code was written to do before the release blocked it. | US, European, Gold | Unchanged |
| [Patrolling staff can cross bridges](staff-cross-bridges/README.md) | Staff walking their rounds can cross a footbridge, like the bridge over the stream in the Lost Kingdom, not only when sent to a job on the other side. | US, European, Gold | Unchanged |

tpw-port carries this repository as a submodule at mods/ and builds every mod in it into the launcher. The launcher's
Mods button lists them, each with a switch, and every one is off until the player turns it on. A build without mods/
has no mods and plays the same.

## The three repositories

The project is three repositories, and every change belongs in exactly one of them:

| Repository | What it holds | Pull requests |
| --- | --- | --- |
| [tpw-decomp](https://github.com/ThemeParkWorldDecomp/tpw-decomp) | The translation: the Mac program's own functions in C++, each behaving exactly as the Mac build does, bugs included, with the differential tests that check it. | None. A function that behaves differently from the Mac is reported as an issue on tpw-port. |
| [tpw-port](https://github.com/ThemeParkWorldDecomp/tpw-port) | The original game on today's Macs and Windows PCs: the launcher, the platform layer, the builds, and the fixes that make it run as it did on the Mac. | Welcome, by its CONTRIBUTING.md. |
| [tpw-mods](https://github.com/ThemeParkWorldDecomp/tpw-mods) | Optional changes to how the game plays, each one off until the player turns it on. tpw-port carries it as a submodule at mods/. | Welcome, by its CONTRIBUTING.md. |

The test for where a change goes: does a park play the same with it on and with it off? If it does, the change belongs in
tpw-port. If it changes the rules, the money, the visitors, the staff, the rides, the advisor, the game's content, what a
control does or the save files, it belongs here, even as a setting that is off by default.

One exception: an option the original game already had in its .sam data files belongs in tpw-port, even when it
changes how a park plays, as long as it offers only the values the original accepts and its default is the original's.
Players set it in the launcher or a game menu, never by editing a .sam file.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).
