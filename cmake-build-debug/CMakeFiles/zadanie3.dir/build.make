# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/nicolas/CLion/clion-2019.2.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/nicolas/CLion/clion-2019.2.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nicolas/CLionProjects/zadanie3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nicolas/CLionProjects/zadanie3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/zadanie3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/zadanie3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zadanie3.dir/flags.make

CMakeFiles/zadanie3.dir/main.c.o: CMakeFiles/zadanie3.dir/flags.make
CMakeFiles/zadanie3.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nicolas/CLionProjects/zadanie3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/zadanie3.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zadanie3.dir/main.c.o   -c /home/nicolas/CLionProjects/zadanie3/main.c

CMakeFiles/zadanie3.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zadanie3.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nicolas/CLionProjects/zadanie3/main.c > CMakeFiles/zadanie3.dir/main.c.i

CMakeFiles/zadanie3.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zadanie3.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nicolas/CLionProjects/zadanie3/main.c -o CMakeFiles/zadanie3.dir/main.c.s

# Object files for target zadanie3
zadanie3_OBJECTS = \
"CMakeFiles/zadanie3.dir/main.c.o"

# External object files for target zadanie3
zadanie3_EXTERNAL_OBJECTS =

zadanie3: CMakeFiles/zadanie3.dir/main.c.o
zadanie3: CMakeFiles/zadanie3.dir/build.make
zadanie3: CMakeFiles/zadanie3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nicolas/CLionProjects/zadanie3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable zadanie3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zadanie3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zadanie3.dir/build: zadanie3

.PHONY : CMakeFiles/zadanie3.dir/build

CMakeFiles/zadanie3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zadanie3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zadanie3.dir/clean

CMakeFiles/zadanie3.dir/depend:
	cd /home/nicolas/CLionProjects/zadanie3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicolas/CLionProjects/zadanie3 /home/nicolas/CLionProjects/zadanie3 /home/nicolas/CLionProjects/zadanie3/cmake-build-debug /home/nicolas/CLionProjects/zadanie3/cmake-build-debug /home/nicolas/CLionProjects/zadanie3/cmake-build-debug/CMakeFiles/zadanie3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zadanie3.dir/depend

