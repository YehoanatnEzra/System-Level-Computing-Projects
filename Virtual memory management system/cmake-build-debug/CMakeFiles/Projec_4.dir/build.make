# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Projec_4.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Projec_4.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Projec_4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Projec_4.dir/flags.make

CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o: CMakeFiles/Projec_4.dir/flags.make
CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o: ../PhysicalMemory.cpp
CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o: CMakeFiles/Projec_4.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o -MF CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o.d -o CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o -c "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/PhysicalMemory.cpp"

CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/PhysicalMemory.cpp" > CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.i

CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/PhysicalMemory.cpp" -o CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.s

CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o: CMakeFiles/Projec_4.dir/flags.make
CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o: ../VirtualMemory.cpp
CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o: CMakeFiles/Projec_4.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o -MF CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o.d -o CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o -c "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/VirtualMemory.cpp"

CMakeFiles/Projec_4.dir/VirtualMemory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Projec_4.dir/VirtualMemory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/VirtualMemory.cpp" > CMakeFiles/Projec_4.dir/VirtualMemory.cpp.i

CMakeFiles/Projec_4.dir/VirtualMemory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Projec_4.dir/VirtualMemory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/VirtualMemory.cpp" -o CMakeFiles/Projec_4.dir/VirtualMemory.cpp.s

CMakeFiles/Projec_4.dir/SimpleTest.cpp.o: CMakeFiles/Projec_4.dir/flags.make
CMakeFiles/Projec_4.dir/SimpleTest.cpp.o: ../SimpleTest.cpp
CMakeFiles/Projec_4.dir/SimpleTest.cpp.o: CMakeFiles/Projec_4.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Projec_4.dir/SimpleTest.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Projec_4.dir/SimpleTest.cpp.o -MF CMakeFiles/Projec_4.dir/SimpleTest.cpp.o.d -o CMakeFiles/Projec_4.dir/SimpleTest.cpp.o -c "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/SimpleTest.cpp"

CMakeFiles/Projec_4.dir/SimpleTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Projec_4.dir/SimpleTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/SimpleTest.cpp" > CMakeFiles/Projec_4.dir/SimpleTest.cpp.i

CMakeFiles/Projec_4.dir/SimpleTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Projec_4.dir/SimpleTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/SimpleTest.cpp" -o CMakeFiles/Projec_4.dir/SimpleTest.cpp.s

# Object files for target Projec_4
Projec_4_OBJECTS = \
"CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o" \
"CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o" \
"CMakeFiles/Projec_4.dir/SimpleTest.cpp.o"

# External object files for target Projec_4
Projec_4_EXTERNAL_OBJECTS =

Projec_4: CMakeFiles/Projec_4.dir/PhysicalMemory.cpp.o
Projec_4: CMakeFiles/Projec_4.dir/VirtualMemory.cpp.o
Projec_4: CMakeFiles/Projec_4.dir/SimpleTest.cpp.o
Projec_4: CMakeFiles/Projec_4.dir/build.make
Projec_4: CMakeFiles/Projec_4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Projec_4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Projec_4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Projec_4.dir/build: Projec_4
.PHONY : CMakeFiles/Projec_4.dir/build

CMakeFiles/Projec_4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Projec_4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Projec_4.dir/clean

CMakeFiles/Projec_4.dir/depend:
	cd "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system" "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system" "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug" "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug" "/mnt/c/Users/yonzr/Desktop/Yehonatan/Huji/year 2/Os/Projects/Projects_to_git/Virtual memory management system/cmake-build-debug/CMakeFiles/Projec_4.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Projec_4.dir/depend

