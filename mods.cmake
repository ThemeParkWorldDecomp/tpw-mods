# The mods of this repository, for the port's build (tpw-port's platform/mods.cmake includes this file). One
# tpw_add_mod per mod: its folder and the sources of its switch-on function. Then its tests, with tpw_add_mod_test. The
# folders are listed in the order the launcher's Mods box shows them. See CONTRIBUTING.md.

# Enable strikes: staff can strike in an open park (the strike_gate hook).
tpw_add_mod(enable-strikes SOURCES enable_strikes.cpp)
tpw_add_mod_test(enable-strikes NAME enable_strikes SOURCES tests/test_enable_strikes.cpp
                 GAME game/buildings/StaffHQ.cpp
                 KEEP __ZN8CStaffHQ13UpdateMonthlyEv   # CStaffHQ::UpdateMonthly()
                      _gStaffHQNoStaff)
