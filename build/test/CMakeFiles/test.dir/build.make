# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/net/桌面/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/net/桌面/server/build

# Include any dependencies generated for this target.
include test/CMakeFiles/test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test.dir/flags.make

test/CMakeFiles/test.dir/test.cpp.o: test/CMakeFiles/test.dir/flags.make
test/CMakeFiles/test.dir/test.cpp.o: /home/net/桌面/server/test/test.cpp
test/CMakeFiles/test.dir/test.cpp.o: test/CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test.dir/test.cpp.o"
	cd /home/net/桌面/server/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test.dir/test.cpp.o -MF CMakeFiles/test.dir/test.cpp.o.d -o CMakeFiles/test.dir/test.cpp.o -c /home/net/桌面/server/test/test.cpp

test/CMakeFiles/test.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/test.cpp.i"
	cd /home/net/桌面/server/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/test/test.cpp > CMakeFiles/test.dir/test.cpp.i

test/CMakeFiles/test.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/test.cpp.s"
	cd /home/net/桌面/server/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/test/test.cpp -o CMakeFiles/test.dir/test.cpp.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/test.cpp.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

/home/net/桌面/server/example/test: test/CMakeFiles/test.dir/test.cpp.o
/home/net/桌面/server/example/test: test/CMakeFiles/test.dir/build.make
/home/net/桌面/server/example/test: /home/net/桌面/server/lib/libmyTinyServer.so
/home/net/桌面/server/example/test: test/CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/net/桌面/server/example/test"
	cd /home/net/桌面/server/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test.dir/build: /home/net/桌面/server/example/test
.PHONY : test/CMakeFiles/test.dir/build

test/CMakeFiles/test.dir/clean:
	cd /home/net/桌面/server/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test.dir/clean

test/CMakeFiles/test.dir/depend:
	cd /home/net/桌面/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/net/桌面/server /home/net/桌面/server/test /home/net/桌面/server/build /home/net/桌面/server/build/test /home/net/桌面/server/build/test/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test.dir/depend

