# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /home/ubuntu/.pyenv/versions/3.10.11/lib/python3.10/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/ubuntu/.pyenv/versions/3.10.11/lib/python3.10/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/LockIn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/LockIn

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/home/ubuntu/.pyenv/versions/3.10.11/lib/python3.10/site-packages/cmake/data/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/ubuntu/.pyenv/versions/3.10.11/lib/python3.10/site-packages/cmake/data/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ubuntu/LockIn/CMakeFiles /home/ubuntu/LockIn//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ubuntu/LockIn/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named main

# Build rule for target.
main: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 main
.PHONY : main

# fast build rule for target.
main/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/build
.PHONY : main/fast

#=============================================================================
# Target rules for targets named test

# Build rule for target.
test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test
.PHONY : test

# fast build rule for target.
test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/build
.PHONY : test/fast

#=============================================================================
# Target rules for targets named test2

# Build rule for target.
test2: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test2
.PHONY : test2

# fast build rule for target.
test2/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test2.dir/build.make CMakeFiles/test2.dir/build
.PHONY : test2/fast

#=============================================================================
# Target rules for targets named test3

# Build rule for target.
test3: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test3
.PHONY : test3

# fast build rule for target.
test3/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test3.dir/build.make CMakeFiles/test3.dir/build
.PHONY : test3/fast

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/main.cpp.s
.PHONY : main.cpp.s

test.o: test.cpp.o
.PHONY : test.o

# target to build an object file
test.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/test.cpp.o
.PHONY : test.cpp.o

test.i: test.cpp.i
.PHONY : test.i

# target to preprocess a source file
test.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/test.cpp.i
.PHONY : test.cpp.i

test.s: test.cpp.s
.PHONY : test.s

# target to generate assembly for a file
test.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/test.cpp.s
.PHONY : test.cpp.s

test2.o: test2.cpp.o
.PHONY : test2.o

# target to build an object file
test2.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test2.dir/build.make CMakeFiles/test2.dir/test2.cpp.o
.PHONY : test2.cpp.o

test2.i: test2.cpp.i
.PHONY : test2.i

# target to preprocess a source file
test2.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test2.dir/build.make CMakeFiles/test2.dir/test2.cpp.i
.PHONY : test2.cpp.i

test2.s: test2.cpp.s
.PHONY : test2.s

# target to generate assembly for a file
test2.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test2.dir/build.make CMakeFiles/test2.dir/test2.cpp.s
.PHONY : test2.cpp.s

test3.o: test3.cpp.o
.PHONY : test3.o

# target to build an object file
test3.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test3.dir/build.make CMakeFiles/test3.dir/test3.cpp.o
.PHONY : test3.cpp.o

test3.i: test3.cpp.i
.PHONY : test3.i

# target to preprocess a source file
test3.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test3.dir/build.make CMakeFiles/test3.dir/test3.cpp.i
.PHONY : test3.cpp.i

test3.s: test3.cpp.s
.PHONY : test3.s

# target to generate assembly for a file
test3.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test3.dir/build.make CMakeFiles/test3.dir/test3.cpp.s
.PHONY : test3.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... main"
	@echo "... test"
	@echo "... test2"
	@echo "... test3"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... test.o"
	@echo "... test.i"
	@echo "... test.s"
	@echo "... test2.o"
	@echo "... test2.i"
	@echo "... test2.s"
	@echo "... test3.o"
	@echo "... test3.i"
	@echo "... test3.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

