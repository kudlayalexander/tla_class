# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/couldlie/vscode_projs/tla2024

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/couldlie/vscode_projs/tla2024/build

# Include any dependencies generated for this target.
include _deps/utils-build/CMakeFiles/utils.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/utils-build/CMakeFiles/utils.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/utils-build/CMakeFiles/utils.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/utils-build/CMakeFiles/utils.dir/flags.make

_deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.o: _deps/utils-build/CMakeFiles/utils.dir/flags.make
_deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.o: _deps/utils-src/src/CRC32.cpp
_deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.o: _deps/utils-build/CMakeFiles/utils.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/couldlie/vscode_projs/tla2024/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.o"
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.o -MF CMakeFiles/utils.dir/src/CRC32.cpp.o.d -o CMakeFiles/utils.dir/src/CRC32.cpp.o -c /home/couldlie/vscode_projs/tla2024/build/_deps/utils-src/src/CRC32.cpp

_deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/utils.dir/src/CRC32.cpp.i"
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/couldlie/vscode_projs/tla2024/build/_deps/utils-src/src/CRC32.cpp > CMakeFiles/utils.dir/src/CRC32.cpp.i

_deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/utils.dir/src/CRC32.cpp.s"
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/couldlie/vscode_projs/tla2024/build/_deps/utils-src/src/CRC32.cpp -o CMakeFiles/utils.dir/src/CRC32.cpp.s

_deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.o: _deps/utils-build/CMakeFiles/utils.dir/flags.make
_deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.o: _deps/utils-src/src/Time.cpp
_deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.o: _deps/utils-build/CMakeFiles/utils.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/couldlie/vscode_projs/tla2024/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object _deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.o"
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.o -MF CMakeFiles/utils.dir/src/Time.cpp.o.d -o CMakeFiles/utils.dir/src/Time.cpp.o -c /home/couldlie/vscode_projs/tla2024/build/_deps/utils-src/src/Time.cpp

_deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/utils.dir/src/Time.cpp.i"
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/couldlie/vscode_projs/tla2024/build/_deps/utils-src/src/Time.cpp > CMakeFiles/utils.dir/src/Time.cpp.i

_deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/utils.dir/src/Time.cpp.s"
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/couldlie/vscode_projs/tla2024/build/_deps/utils-src/src/Time.cpp -o CMakeFiles/utils.dir/src/Time.cpp.s

# Object files for target utils
utils_OBJECTS = \
"CMakeFiles/utils.dir/src/CRC32.cpp.o" \
"CMakeFiles/utils.dir/src/Time.cpp.o"

# External object files for target utils
utils_EXTERNAL_OBJECTS =

_deps/utils-build/libutils.a: _deps/utils-build/CMakeFiles/utils.dir/src/CRC32.cpp.o
_deps/utils-build/libutils.a: _deps/utils-build/CMakeFiles/utils.dir/src/Time.cpp.o
_deps/utils-build/libutils.a: _deps/utils-build/CMakeFiles/utils.dir/build.make
_deps/utils-build/libutils.a: _deps/utils-build/CMakeFiles/utils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/couldlie/vscode_projs/tla2024/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libutils.a"
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean_target.cmake
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/utils-build/CMakeFiles/utils.dir/build: _deps/utils-build/libutils.a
.PHONY : _deps/utils-build/CMakeFiles/utils.dir/build

_deps/utils-build/CMakeFiles/utils.dir/clean:
	cd /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean.cmake
.PHONY : _deps/utils-build/CMakeFiles/utils.dir/clean

_deps/utils-build/CMakeFiles/utils.dir/depend:
	cd /home/couldlie/vscode_projs/tla2024/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/couldlie/vscode_projs/tla2024 /home/couldlie/vscode_projs/tla2024/build/_deps/utils-src /home/couldlie/vscode_projs/tla2024/build /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build /home/couldlie/vscode_projs/tla2024/build/_deps/utils-build/CMakeFiles/utils.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : _deps/utils-build/CMakeFiles/utils.dir/depend
