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
include test/CMakeFiles/thread.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/thread.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/thread.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/thread.dir/flags.make

test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.o: test/CMakeFiles/thread.dir/flags.make
test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.o: /home/net/桌面/server/test/ThreadPoolTest.cpp
test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.o: test/CMakeFiles/thread.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.o"
	cd /home/net/桌面/server/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.o -MF CMakeFiles/thread.dir/ThreadPoolTest.cpp.o.d -o CMakeFiles/thread.dir/ThreadPoolTest.cpp.o -c /home/net/桌面/server/test/ThreadPoolTest.cpp

test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread.dir/ThreadPoolTest.cpp.i"
	cd /home/net/桌面/server/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/test/ThreadPoolTest.cpp > CMakeFiles/thread.dir/ThreadPoolTest.cpp.i

test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread.dir/ThreadPoolTest.cpp.s"
	cd /home/net/桌面/server/build/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/test/ThreadPoolTest.cpp -o CMakeFiles/thread.dir/ThreadPoolTest.cpp.s

# Object files for target thread
thread_OBJECTS = \
"CMakeFiles/thread.dir/ThreadPoolTest.cpp.o"

# External object files for target thread
thread_EXTERNAL_OBJECTS =

/home/net/桌面/server/example/thread: test/CMakeFiles/thread.dir/ThreadPoolTest.cpp.o
/home/net/桌面/server/example/thread: test/CMakeFiles/thread.dir/build.make
/home/net/桌面/server/example/thread: /home/net/桌面/server/lib/libmyTinyServer.so
/home/net/桌面/server/example/thread: test/CMakeFiles/thread.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/net/桌面/server/example/thread"
	cd /home/net/桌面/server/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/thread.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/thread.dir/build: /home/net/桌面/server/example/thread
.PHONY : test/CMakeFiles/thread.dir/build

test/CMakeFiles/thread.dir/clean:
	cd /home/net/桌面/server/build/test && $(CMAKE_COMMAND) -P CMakeFiles/thread.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/thread.dir/clean

test/CMakeFiles/thread.dir/depend:
	cd /home/net/桌面/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/net/桌面/server /home/net/桌面/server/test /home/net/桌面/server/build /home/net/桌面/server/build/test /home/net/桌面/server/build/test/CMakeFiles/thread.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/thread.dir/depend

