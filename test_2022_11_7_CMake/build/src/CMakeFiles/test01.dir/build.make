# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sjw/projects/test_2022_11_7_CMake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sjw/projects/test_2022_11_7_CMake/build

# Include any dependencies generated for this target.
include src/CMakeFiles/test01.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/test01.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/test01.dir/flags.make

src/CMakeFiles/test01.dir/test.cc.o: src/CMakeFiles/test01.dir/flags.make
src/CMakeFiles/test01.dir/test.cc.o: ../src/test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sjw/projects/test_2022_11_7_CMake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/test01.dir/test.cc.o"
	cd /home/sjw/projects/test_2022_11_7_CMake/build/src && /opt/rh/devtoolset-7/root/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test01.dir/test.cc.o -c /home/sjw/projects/test_2022_11_7_CMake/src/test.cc

src/CMakeFiles/test01.dir/test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test01.dir/test.cc.i"
	cd /home/sjw/projects/test_2022_11_7_CMake/build/src && /opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sjw/projects/test_2022_11_7_CMake/src/test.cc > CMakeFiles/test01.dir/test.cc.i

src/CMakeFiles/test01.dir/test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test01.dir/test.cc.s"
	cd /home/sjw/projects/test_2022_11_7_CMake/build/src && /opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sjw/projects/test_2022_11_7_CMake/src/test.cc -o CMakeFiles/test01.dir/test.cc.s

src/CMakeFiles/test01.dir/test.cc.o.requires:

.PHONY : src/CMakeFiles/test01.dir/test.cc.o.requires

src/CMakeFiles/test01.dir/test.cc.o.provides: src/CMakeFiles/test01.dir/test.cc.o.requires
	$(MAKE) -f src/CMakeFiles/test01.dir/build.make src/CMakeFiles/test01.dir/test.cc.o.provides.build
.PHONY : src/CMakeFiles/test01.dir/test.cc.o.provides

src/CMakeFiles/test01.dir/test.cc.o.provides.build: src/CMakeFiles/test01.dir/test.cc.o


# Object files for target test01
test01_OBJECTS = \
"CMakeFiles/test01.dir/test.cc.o"

# External object files for target test01
test01_EXTERNAL_OBJECTS =

../lib/libtest01.so: src/CMakeFiles/test01.dir/test.cc.o
../lib/libtest01.so: src/CMakeFiles/test01.dir/build.make
../lib/libtest01.so: src/CMakeFiles/test01.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sjw/projects/test_2022_11_7_CMake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libtest01.so"
	cd /home/sjw/projects/test_2022_11_7_CMake/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test01.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/test01.dir/build: ../lib/libtest01.so

.PHONY : src/CMakeFiles/test01.dir/build

src/CMakeFiles/test01.dir/requires: src/CMakeFiles/test01.dir/test.cc.o.requires

.PHONY : src/CMakeFiles/test01.dir/requires

src/CMakeFiles/test01.dir/clean:
	cd /home/sjw/projects/test_2022_11_7_CMake/build/src && $(CMAKE_COMMAND) -P CMakeFiles/test01.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/test01.dir/clean

src/CMakeFiles/test01.dir/depend:
	cd /home/sjw/projects/test_2022_11_7_CMake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sjw/projects/test_2022_11_7_CMake /home/sjw/projects/test_2022_11_7_CMake/src /home/sjw/projects/test_2022_11_7_CMake/build /home/sjw/projects/test_2022_11_7_CMake/build/src /home/sjw/projects/test_2022_11_7_CMake/build/src/CMakeFiles/test01.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/test01.dir/depend
