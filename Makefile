# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jlc/projects/BMC_MultiDM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jlc/projects/BMC_MultiDM

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/jlc/projects/BMC_MultiDM/CMakeFiles /home/jlc/projects/BMC_MultiDM/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/jlc/projects/BMC_MultiDM/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named bmcusb

# Build rule for target.
bmcusb: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 bmcusb
.PHONY : bmcusb

# fast build rule for target.
bmcusb/fast:
	$(MAKE) -f src/CMakeFiles/bmcusb.dir/build.make src/CMakeFiles/bmcusb.dir/build
.PHONY : bmcusb/fast

#=============================================================================
# Target rules for targets named playBMC

# Build rule for target.
playBMC: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 playBMC
.PHONY : playBMC

# fast build rule for target.
playBMC/fast:
	$(MAKE) -f src/CMakeFiles/playBMC.dir/build.make src/CMakeFiles/playBMC.dir/build
.PHONY : playBMC/fast

#=============================================================================
# Target rules for targets named playMorph

# Build rule for target.
playMorph: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 playMorph
.PHONY : playMorph

# fast build rule for target.
playMorph/fast:
	$(MAKE) -f src/CMakeFiles/playMorph.dir/build.make src/CMakeFiles/playMorph.dir/build
.PHONY : playMorph/fast

#=============================================================================
# Target rules for targets named testbmcusb

# Build rule for target.
testbmcusb: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 testbmcusb
.PHONY : testbmcusb

# fast build rule for target.
testbmcusb/fast:
	$(MAKE) -f src/CMakeFiles/testbmcusb.dir/build.make src/CMakeFiles/testbmcusb.dir/build
.PHONY : testbmcusb/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... bmcusb"
	@echo "... playBMC"
	@echo "... playMorph"
	@echo "... testbmcusb"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

