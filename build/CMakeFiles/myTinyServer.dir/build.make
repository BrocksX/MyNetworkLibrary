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
include CMakeFiles/myTinyServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/myTinyServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/myTinyServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myTinyServer.dir/flags.make

CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o: /home/net/桌面/server/src/base/ThreadPool.cpp
CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o -MF CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o -c /home/net/桌面/server/src/base/ThreadPool.cpp

CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/base/ThreadPool.cpp > CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.i

CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/base/ThreadPool.cpp -o CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.s

CMakeFiles/myTinyServer.dir/src/base/util.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/base/util.cpp.o: /home/net/桌面/server/src/base/util.cpp
CMakeFiles/myTinyServer.dir/src/base/util.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/myTinyServer.dir/src/base/util.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/base/util.cpp.o -MF CMakeFiles/myTinyServer.dir/src/base/util.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/base/util.cpp.o -c /home/net/桌面/server/src/base/util.cpp

CMakeFiles/myTinyServer.dir/src/base/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/base/util.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/base/util.cpp > CMakeFiles/myTinyServer.dir/src/base/util.cpp.i

CMakeFiles/myTinyServer.dir/src/base/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/base/util.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/base/util.cpp -o CMakeFiles/myTinyServer.dir/src/base/util.cpp.s

CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o: /home/net/桌面/server/src/net/Acceptor.cpp
CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o -c /home/net/桌面/server/src/net/Acceptor.cpp

CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/Acceptor.cpp > CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.i

CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/Acceptor.cpp -o CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.s

CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o: /home/net/桌面/server/src/net/Buffer.cpp
CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o -c /home/net/桌面/server/src/net/Buffer.cpp

CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/Buffer.cpp > CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.i

CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/Buffer.cpp -o CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.s

CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o: /home/net/桌面/server/src/net/Channel.cpp
CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o -c /home/net/桌面/server/src/net/Channel.cpp

CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/Channel.cpp > CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.i

CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/Channel.cpp -o CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.s

CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o: /home/net/桌面/server/src/net/Connection.cpp
CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o -c /home/net/桌面/server/src/net/Connection.cpp

CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/Connection.cpp > CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.i

CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/Connection.cpp -o CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.s

CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o: /home/net/桌面/server/src/net/Epoll.cpp
CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o -c /home/net/桌面/server/src/net/Epoll.cpp

CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/Epoll.cpp > CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.i

CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/Epoll.cpp -o CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.s

CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o: /home/net/桌面/server/src/net/EventLoop.cpp
CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o -c /home/net/桌面/server/src/net/EventLoop.cpp

CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/EventLoop.cpp > CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.i

CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/EventLoop.cpp -o CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.s

CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o: /home/net/桌面/server/src/net/Server.cpp
CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o -c /home/net/桌面/server/src/net/Server.cpp

CMakeFiles/myTinyServer.dir/src/net/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/Server.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/Server.cpp > CMakeFiles/myTinyServer.dir/src/net/Server.cpp.i

CMakeFiles/myTinyServer.dir/src/net/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/Server.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/Server.cpp -o CMakeFiles/myTinyServer.dir/src/net/Server.cpp.s

CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o: CMakeFiles/myTinyServer.dir/flags.make
CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o: /home/net/桌面/server/src/net/Socket.cpp
CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o: CMakeFiles/myTinyServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o -MF CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o.d -o CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o -c /home/net/桌面/server/src/net/Socket.cpp

CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/net/桌面/server/src/net/Socket.cpp > CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.i

CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/net/桌面/server/src/net/Socket.cpp -o CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.s

# Object files for target myTinyServer
myTinyServer_OBJECTS = \
"CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/base/util.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o" \
"CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o"

# External object files for target myTinyServer
myTinyServer_EXTERNAL_OBJECTS =

/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/base/ThreadPool.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/base/util.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/Acceptor.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/Buffer.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/Channel.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/Connection.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/Epoll.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/EventLoop.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/Server.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/src/net/Socket.cpp.o
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/build.make
/home/net/桌面/server/lib/libmyTinyServer.so: CMakeFiles/myTinyServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/net/桌面/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX shared library /home/net/桌面/server/lib/libmyTinyServer.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myTinyServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myTinyServer.dir/build: /home/net/桌面/server/lib/libmyTinyServer.so
.PHONY : CMakeFiles/myTinyServer.dir/build

CMakeFiles/myTinyServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myTinyServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myTinyServer.dir/clean

CMakeFiles/myTinyServer.dir/depend:
	cd /home/net/桌面/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/net/桌面/server /home/net/桌面/server /home/net/桌面/server/build /home/net/桌面/server/build /home/net/桌面/server/build/CMakeFiles/myTinyServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myTinyServer.dir/depend

