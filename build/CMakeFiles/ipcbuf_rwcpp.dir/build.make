# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /usr/local/lib/python3.9/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /usr/local/lib/python3.9/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspace/psrdada_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/psrdada_test/build

# Include any dependencies generated for this target.
include CMakeFiles/ipcbuf_rwcpp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ipcbuf_rwcpp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ipcbuf_rwcpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ipcbuf_rwcpp.dir/flags.make

CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o: CMakeFiles/ipcbuf_rwcpp.dir/flags.make
CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o: /workspace/psrdada_test/src/ipcbuf_rw.cpp
CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o: CMakeFiles/ipcbuf_rwcpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/psrdada_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o -MF CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o.d -o CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o -c /workspace/psrdada_test/src/ipcbuf_rw.cpp

CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/psrdada_test/src/ipcbuf_rw.cpp > CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.i

CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/psrdada_test/src/ipcbuf_rw.cpp -o CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.s

# Object files for target ipcbuf_rwcpp
ipcbuf_rwcpp_OBJECTS = \
"CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o"

# External object files for target ipcbuf_rwcpp
ipcbuf_rwcpp_EXTERNAL_OBJECTS =

ipcbuf_rwcpp: CMakeFiles/ipcbuf_rwcpp.dir/src/ipcbuf_rw.cpp.o
ipcbuf_rwcpp: CMakeFiles/ipcbuf_rwcpp.dir/build.make
ipcbuf_rwcpp: CMakeFiles/ipcbuf_rwcpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/workspace/psrdada_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ipcbuf_rwcpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ipcbuf_rwcpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ipcbuf_rwcpp.dir/build: ipcbuf_rwcpp
.PHONY : CMakeFiles/ipcbuf_rwcpp.dir/build

CMakeFiles/ipcbuf_rwcpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ipcbuf_rwcpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ipcbuf_rwcpp.dir/clean

CMakeFiles/ipcbuf_rwcpp.dir/depend:
	cd /workspace/psrdada_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/psrdada_test /workspace/psrdada_test /workspace/psrdada_test/build /workspace/psrdada_test/build /workspace/psrdada_test/build/CMakeFiles/ipcbuf_rwcpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ipcbuf_rwcpp.dir/depend

