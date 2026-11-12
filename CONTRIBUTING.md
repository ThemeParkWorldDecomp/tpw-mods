# Contributing

This repository holds optional changes to how the game plays: the rules, the money, the visitors, the staff, the rides,
the advisor, the game's content, what a control does or the save files. A change that keeps a park playing the same
(a fix, another system, a display or launcher setting) belongs in [tpw-port](https://github.com/ThemeParkWorldDecomp/tpw-port),
and so does an option the original game already had in its .sam data files, offered with only the values the original
accepts and set in the launcher or a game menu. tpw-decomp takes no changes. See the [README](README.md) for the three repositories.

## What a mod needs

- Its own folder, `<id>/` at the top of this repository, with a mod.json (below), its code and its tests, and a line
  in mods.cmake.
- Original code under the license named in mod.json. No files of the game, and nothing copied from them.
- Tests with the mod off and with it on. With it off, the game must behave exactly as the original.
- One mod per pull request. Say what you tested and on what: the edition (US, European, Gold) and the system.

## How the pieces fit

[Enable strikes](enable-strikes/) is a complete example; copy its folder to start a new mod.

**mod.json** describes the mod. The launcher's Mods box shows it, and the port's build reads and checks it (a mistake
stops the build with the file and the field named):

| Field | What it holds |
| --- | --- |
| `id` | The folder's name: lower case letters and digits joined by `-`. The player's choice is kept in launcher.ini as `mod.<id>=1`. |
| `name` | The name players see. |
| `version` | `X.Y.Z`. Raise it with every change. |
| `author` | Who wrote it. |
| `license` | The license of its code. |
| `text` | What it does, for players: one or two plain sentences. |
| `editions` | Those it works with, from `"US"`, `"European"` and `"Gold"`. |
| `conflicts` | The ids of mods it cannot be on with (both are checked). `[]` for none. |
| `changes_saves` | `true` when a park saved with it on differs from the original's saves. The Mods box then warns the player. |
| `hooks` | The hooks it switches on (below). |
| `content` | Optional. For a mod that adds the game's content made from the player's own disc (never from this repository): the folder of the launcher's data folder's assets that holds it, such as `"ps1"`. Rides go in it as in the game folder, each a folder `data/levels/<theme>/<rides, features or sideshow>/<name>/` with its `<name>.sam`, whose `Info.DownloadedRideNumber` is 201 to 256 and its own. With the mod on, the launcher lays them over the game folder; the Mods box shows the mod locked until the folder holds a ride. Such a mod has `changes_saves` `true`. |

**Hooks** are the only way a mod changes the game. A hook is a small point in tpw-port's copy of the game's code, one
of its fixes (a row of its FIXES.md that names `TPW_MOD_HOOK <name>`), that does exactly what the original does unless
a mod switches it on. tpw-port lists them in platform/mods.cmake (`TPW_MOD_HOOKS`) and platform/mods.h (`struct Hooks`, one
field each). The hooks so far:

| Hook | Where | Switched on |
| --- | --- | --- |
| `strike_gate` | CStaffHQ::UpdateMonthly | Strikes are considered in any open park, not only in one with no visitors. |
| `strike_messages` | CStaffHQ::StopStrikeInProgress, CParkTime::Update (CStaffHQ::ModEndOfDay) | The advisor's two unused strike messages: "strike over" when a strike ends, and a "last warning" seven days before the month-end check to each job that would then strike and still has its grievance. |
| `ps1_badge` | UpdateViewWindow (src/game/ui/ui_ridepanel.cpp) | The PlayStation rides (the mod's content) have a "PS1" tag on the Buy Ride panel's picture while the launcher's "Show Ride Badges" is on. Display only. |
| `ps1_ground` | CalculateSkirtVertices (src/game/park/SkirtDeform.cpp) | Each cell of a PlayStation ride's own ground (the mod's content) is blended into the park's ground under it, as a PC ride's base is, while the game's Ground blending option is on. Display only. |
| `ps1_save_repair` | CSaveLoad::PostLoadOperations (src/game/save/SaveLoad.cpp) | As a park loads, each PlayStation ride (the mod's content) whose saved entrance and exit are not on the cells its ride file now gives has them moved there: the old queue becomes a path, the new exit is joined to a path and the new entrance gets a queue joined to one, or is left to be built where no path is in reach. A ride whose doors are already right is left alone. |
| `ps1_pick_rotation` | PlaceRide (src/game/ui/ui_ridepanel.cpp) | A PlayStation ride (the mod's content) picked from Buy Ride starts half a turn round, so its entrance comes up at the bottom at the default camera, as a PC ride's does. Picking again while a ride is held keeps the player's own turn. Rides already placed and saved parks are not touched. |
| `tpi_worlds` | TpiWorldOfTheme, CGame::GetLocalisedThemeGateNames, ConverttoCurrentTheme, CGame::GetLocalisedThemeName (src/game/Game.cpp), LoadLevelAdvisors (src/game/TPLevel.cpp), CRideDatabase::GetTypeName, GetSignNameA, GetSignNameB and its record load (src/game/rides/RideDB.cpp), CFSGlobalMasterSave::HasAllResearchBeenCompleted, CFSGlobalParkData::GetSignNameA, GetSignNameB, GetFullName (src/game/save/FS_MasterSave.cpp), CFSOnlineSystem::RequestUpload (src/game/online/FS_Online.cpp), TGALoadAndConvert (src/engine/TextureLoadManager.cpp) | The game's worlds are the original four and the three made from the player's Theme Park Inc copy (the mod's content, include/TpiWorlds.h): their titles on the island screen, their gate signs and park names, the names of their rides from each ride's own file, the Settings box's theme choice found by folder name so the four keep their places, the advisor's models from the donor world, a park in one of them refused for upload, the textures of their files blended as their soft alpha asks, and with the hook off their master-save records passed over. |
| `tpi_islands` | ParseThemeDescriptor (src/game/online/lobby_parser.cpp), GotoIsland, TickIslands and CLBScreenComponentThemeIslands::WindowMessagesFn (src/game/online/lobby_island.cpp), with the ring's pure functions in include/tpi_island_ring.h | The island screen has a second ring of islands for the three worlds (the mod's content: the converter makes each world's island from the world's own gate, terrain and textures), north of the original four and out of their fog; Tab, or a right click on the view, flies the camera between the rings, the arrows and the panel's buttons browse the ring in view, and the original four islands stay where they are. |

**The code** is one function, `tpw::mods::SwitchOn_<id>` with each `-` of the id written `_` (for enable-strikes,
`SwitchOn_enable_strikes`). It takes the `Hooks` and sets the fields of the hooks the mod uses. The launcher calls it
before the game starts, only when the mod is on and is for the edition being played.

**mods.cmake** lists the mods, in the order the Mods box shows them, with two functions of tpw-port's
platform/mods.cmake:

- `tpw_add_mod(<id> SOURCES <files>)`: the mod's folder and the files of its code.
- `tpw_add_mod_test(<id> NAME <name> SOURCES <files> [GAME <files under src/> KEEP <symbols>])`: a test program, run by
  `ctest` as `mod_<name>`. It is built from its own files and the mod's code. GAME compiles in files of the game's code,
  cut to the functions KEEP names (their mangled names), and the test supplies everything they call.

To build with a copy of this repository, check it out as tpw-port's mods/ (or pass `-DTPW_MODS_DIR=<folder>`) and
build tpw-port as its README says. The configure step prints a line for each mod it found.

## How a mod lands

A mod uses the hooks above: small, reviewed points in the game's code that do nothing unless a mod is on. If your mod
needs a hook that does not exist, describe it in the pull request; the maintainers add it to tpw-port. Never change the
translation or its tests to make a mod work. An accepted mod is merged here, and tpw-port then moves its mods/ submodule
to the reviewed commit.
