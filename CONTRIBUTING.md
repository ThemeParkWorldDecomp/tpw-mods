# Contributing

This repository holds optional changes to how the game plays: the rules, the money, the visitors, the staff, the rides,
the advisor, the game's content, what a control does or the save files. A change that keeps a park playing the same
(a fix, another system, a display or launcher setting) belongs in [tpw-port](https://github.com/ThemeParkWorldDecomp/tpw-port),
and tpw-decomp takes no changes. See the [README](README.md) for the three repositories.

## What a mod needs

- Its own folder, `<id>/` at the top of this repository, with a mod.json: the id, name, version, author, license, a short explanation for
  players, the editions it supports, any mods it conflicts with, and whether it changes saved games.
- Original code under the license named in mod.json. No files of the game, and nothing copied from them.
- Tests with the mod off and with it on. With it off, the game must behave exactly as the original.
- One mod per pull request. Say what you tested and on what: the edition (US, European, Gold) and the system.

## How a mod lands

A mod uses hooks in tpw-port: small, reviewed points in the game's code that do nothing unless a mod is on. If your mod
needs a hook that does not exist, describe it in the pull request; the maintainers add it to tpw-port. Never change the
translation or its tests to make a mod work. An accepted mod is merged here, and tpw-port then moves its mods/ submodule
to the reviewed commit.
