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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp

# Include any dependencies generated for this target.
include CMakeFiles/cmTC_64a60.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cmTC_64a60.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cmTC_64a60.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cmTC_64a60.dir/flags.make

CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o: CMakeFiles/cmTC_64a60.dir/flags.make
CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o: /usr/share/cmake-3.22/Modules/CMakeCXXCompilerABI.cpp
CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o: CMakeFiles/cmTC_64a60.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=/mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o -MF CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o.d -o CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o -c /usr/share/cmake-3.22/Modules/CMakeCXXCompilerABI.cpp

CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.i: cmake_force
	@echo "Preprocessing CXX source to CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/share/cmake-3.22/Modules/CMakeCXXCompilerABI.cpp > CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.i

CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.s: cmake_force
	@echo "Compiling CXX source to assembly CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/share/cmake-3.22/Modules/CMakeCXXCompilerABI.cpp -o CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.s

# Object files for target cmTC_64a60
cmTC_64a60_OBJECTS = \
"CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o"

# External object files for target cmTC_64a60
cmTC_64a60_EXTERNAL_OBJECTS =

cmTC_64a60: CMakeFiles/cmTC_64a60.dir/CMakeCXXCompilerABI.cpp.o
cmTC_64a60: CMakeFiles/cmTC_64a60.dir/build.make
cmTC_64a60: CMakeFiles/cmTC_64a60.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=/mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cmTC_64a60"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmTC_64a60.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cmTC_64a60.dir/build: cmTC_64a60
.PHONY : CMakeFiles/cmTC_64a60.dir/build

CMakeFiles/cmTC_64a60.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cmTC_64a60.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cmTC_64a60.dir/clean

CMakeFiles/cmTC_64a60.dir/depend:
	cd /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp /mnt/c/Users/LenY/dev/uni/sem_2/nagyhz_2/CMakeFiles/CMakeTmp/CMakeFiles/cmTC_64a60.dir/DependInfo.cmake
.PHONY : CMakeFiles/cmTC_64a60.dir/depend

