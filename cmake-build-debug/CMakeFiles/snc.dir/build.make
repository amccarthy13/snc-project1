# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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
CMAKE_COMMAND = /Users/andrewM/Desktop/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Users/andrewM/Desktop/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/andrewM/CLionProjects/snc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/andrewM/CLionProjects/snc/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/snc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/snc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/snc.dir/flags.make

CMakeFiles/snc.dir/snc.c.o: CMakeFiles/snc.dir/flags.make
CMakeFiles/snc.dir/snc.c.o: ../snc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andrewM/CLionProjects/snc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/snc.dir/snc.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/snc.dir/snc.c.o   -c /Users/andrewM/CLionProjects/snc/snc.c

CMakeFiles/snc.dir/snc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/snc.dir/snc.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andrewM/CLionProjects/snc/snc.c > CMakeFiles/snc.dir/snc.c.i

CMakeFiles/snc.dir/snc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/snc.dir/snc.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andrewM/CLionProjects/snc/snc.c -o CMakeFiles/snc.dir/snc.c.s

CMakeFiles/snc.dir/snc.c.o.requires:

.PHONY : CMakeFiles/snc.dir/snc.c.o.requires

CMakeFiles/snc.dir/snc.c.o.provides: CMakeFiles/snc.dir/snc.c.o.requires
	$(MAKE) -f CMakeFiles/snc.dir/build.make CMakeFiles/snc.dir/snc.c.o.provides.build
.PHONY : CMakeFiles/snc.dir/snc.c.o.provides

CMakeFiles/snc.dir/snc.c.o.provides.build: CMakeFiles/snc.dir/snc.c.o


# Object files for target snc
snc_OBJECTS = \
"CMakeFiles/snc.dir/snc.c.o"

# External object files for target snc
snc_EXTERNAL_OBJECTS =

snc: CMakeFiles/snc.dir/snc.c.o
snc: CMakeFiles/snc.dir/build.make
snc: CMakeFiles/snc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/andrewM/CLionProjects/snc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable snc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/snc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/snc.dir/build: snc

.PHONY : CMakeFiles/snc.dir/build

CMakeFiles/snc.dir/requires: CMakeFiles/snc.dir/snc.c.o.requires

.PHONY : CMakeFiles/snc.dir/requires

CMakeFiles/snc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/snc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/snc.dir/clean

CMakeFiles/snc.dir/depend:
	cd /Users/andrewM/CLionProjects/snc/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/andrewM/CLionProjects/snc /Users/andrewM/CLionProjects/snc /Users/andrewM/CLionProjects/snc/cmake-build-debug /Users/andrewM/CLionProjects/snc/cmake-build-debug /Users/andrewM/CLionProjects/snc/cmake-build-debug/CMakeFiles/snc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/snc.dir/depend

