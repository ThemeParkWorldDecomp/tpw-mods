# The mods of this repository, for the port's build (tpw-port's platform/mods.cmake includes this file). One
# tpw_add_mod per mod: its folder and the sources of its switch-on function. Then its tests, with tpw_add_mod_test. The
# launcher's Mods box shows the mods in their groups (mod.json's "category": Bonus Content, then Gameplay), each group
# in this file's order. See CONTRIBUTING.md.

# Enable strikes: staff can strike in an open park (the strike_gate hook), and the advisor gives the original's two
# unused strike messages (the strike_messages hook).
tpw_add_mod(enable-strikes SOURCES enable_strikes.cpp)
tpw_add_mod_test(enable-strikes NAME enable_strikes SOURCES tests/test_enable_strikes.cpp
                 GAME game/buildings/StaffHQ.cpp
                 KEEP __ZN8CStaffHQ13UpdateMonthlyEv   # CStaffHQ::UpdateMonthly()
                      _gStaffHQNoStaff)
tpw_add_mod_test(enable-strikes NAME enable_strikes_messages SOURCES tests/test_enable_strikes_messages.cpp
                 GAME game/buildings/StaffHQ.cpp game/people/ParkTime.cpp
                 KEEP __ZN8CStaffHQ20StopStrikeInProgressE9StaffType   # CStaffHQ::StopStrikeInProgress(StaffType)
                      __ZN8CStaffHQ11ModEndOfDayEv                     # CStaffHQ::ModEndOfDay()
                      __ZN9CParkTime6UpdateEv                          # CParkTime::Update()
                      _gStaffHQNoStaff
                      _gParkDate)

# Patrolling staff can cross bridges: a staff member's random walk may step onto, along and off a footbridge (the
# staff_bridge_wander hook).
tpw_add_mod(staff-cross-bridges SOURCES staff_cross_bridges.cpp)
tpw_add_mod_test(staff-cross-bridges NAME staff_cross_bridges SOURCES tests/test_staff_cross_bridges.cpp
                 GAME game/people/Peep.cpp
                 KEEP __ZN5CPeep20SetRandomDestinationEv   # CPeep::SetRandomDestination()
                      _gPeepNoCell)

# Better land buying: a land purchase's sides may cross rivers, lakes and rocks (the land_rough_edges hook), it may be one
# fence square wide (land_thin_strips), land it closes in stays unowned and fenced until bought (land_unbought), and it costs
# the unowned land inside its new fence line, rivers, lakes and rocks free (land_fair_price).
tpw_add_mod(better-land-buying SOURCES better_land_buying.cpp)
tpw_add_mod_test(better-land-buying NAME better_land_buying SOURCES tests/test_better_land_buying.cpp
                 GAME game/park/LLCellSystem.cpp
                 KEEP __Z24LLCC_IsHoardingEditLegaliiiib       # LLCC_IsHoardingEditLegal(int, int, int, int, bool)
                      __Z16LLCC_SoIsItLegalv                   # LLCC_SoIsItLegal()
                      __Z14LLCC_FloodFilliihh                  # LLCC_FloodFill(int, int, unsigned char, unsigned char)
                      __Z26LLCC_CopyCurrentToNewImagev         # LLCC_CopyCurrentToNewImage()
                      __Z22LLCC_AddNewAreaToImageiiii          # LLCC_AddNewAreaToImage(int, int, int, int)
                      __Z31LLCC_CreateCurrentHoardingImagev    # LLCC_CreateCurrentHoardingImage()
                      _gHoardingLeft _gHoardingTop _gHoardingRight _gHoardingBottom _gLLCCWidth _gLLCCHeight _gLLCC4818
                      _gLLCC47CC _gLLCCC204 _gLLCCEdgeCount _gLLCCEdgeMarks _gLLCCHoardingImages)
# Each hook alone, including land_rough_edges in an original world: a rectangle round the gate is refused.
add_test(NAME mod_better_land_buying_gate COMMAND tpw_mod_test_better_land_buying --gate-only)
get_test_property(mod_better_land_buying DISABLED land_buying_test_disabled)
if(land_buying_test_disabled)
  set_tests_properties(mod_better_land_buying_gate PROPERTIES DISABLED ON)
endif()
tpw_add_mod_test(better-land-buying NAME better_land_buying_fence SOURCES tests/test_better_land_buying_fence.cpp
                 GAME game/park/LLCellSystem.cpp game/park/LLCellRide.cpp
                 KEEP __Z19LLCC_ReviseHoardingiiii                    # LLCC_ReviseHoarding(int, int, int, int)
                      __Z17LLCC_LockHoardingb                         # LLCC_LockHoarding(bool)
                      __Z21LLCC_FillInsideOfParkv                     # LLCC_FillInsideOfPark()
                      __Z31LLCC_CreateCurrentHoardingImagev           # LLCC_CreateCurrentHoardingImage()
                      __Z20LLCC_BuildHoardingDBiiiicb                 # LLCC_BuildHoardingDB(int, int, int, int, char, bool)
                      __ZN3tpw4land14UnboughtBeforeEiiii              # tpw::land::UnboughtBefore(int, int, int, int)
                      __ZN3tpw4land15UnboughtPrepareEv                # tpw::land::UnboughtPrepare()
                      __ZN3tpw4land18UnboughtKeepsFenceEPK10CTrackCell   # tpw::land::UnboughtKeepsFence(CTrackCell const*)
                      __ZN3tpw4land13UnboughtAfterEv                  # tpw::land::UnboughtAfter()
                      _gHoardingLeft _gHoardingTop _gHoardingRight _gHoardingBottom _gLLCCWidth _gLLCCHeight _gLLCC4818
                      _gLLCCE0CC _gLLCCHoardingImages _gLLCCReviseAnswer
                      _gLLCCHoardMinX _gLLCCHoardMinY _gLLCCHoardMaxX _gLLCCHoardMaxY)

# PlayStation rides: its rides are content made from the player's own PlayStation disc (mod.json's "content", the data
# folder's assets/ps1), which the launcher lays over the game folder with the mod on; the ps1_badge hook tags them "PS1"
# on the Buy Ride panel while the launcher's "Show Ride Badges" is on, the ps1_ground hook blends their own ground
# into the park's ground, the ps1_save_repair hook moves the doors of rides placed with launcher 0.4.0's conversion as a
# park loads, and the ps1_pick_rotation hook starts each of its rides picked from Buy Ride half a turn round, its
# entrance at the bottom like a PC ride's.
tpw_add_mod(ps1-rides SOURCES ps1_rides.cpp)

# TPI Worlds and Rides: three more worlds (Arabian Nights, Polar Zone, Land of Invention), content made from the
# player's own copy of Theme Park Inc (mod.json's "content", the data folder's assets/tpi), which the launcher lays over
# the game folder with the mod on; the tpi_worlds hook adds them to the game's list of worlds and the tpi_islands hook
# gives the island screen a second group of islands for them.
tpw_add_mod(tpi-worlds SOURCES tpi_worlds.cpp)

# Entertainers' second acts: the eight costumes whose sprite files hold a finished second act that no custom block names
# perform it at random as well as their first (the entertainer_second_acts hook).
tpw_add_mod(entertainer-second-acts SOURCES entertainer_second_acts.cpp)
tpw_add_mod_test(entertainer-second-acts NAME entertainer_second_acts SOURCES tests/test_entertainer_second_acts.cpp
                 GAME game/people/Entertainer.cpp
                 KEEP __ZN12CEntertainer8SetStateEN6CStaff10StaffStateE)   # CEntertainer::SetState(CStaff::StaffState)

# PS2 Guest Models (3D): the PlayStation 2 edition's 3D models of the children, made from the player's own PlayStation 2
# disc (mod.json's "content", the data folder's assets/ps2); the ps2_characters hook draws them in the sprites' place.
# Display only.
tpw_add_mod(ps2-characters SOURCES ps2_characters.cpp)
