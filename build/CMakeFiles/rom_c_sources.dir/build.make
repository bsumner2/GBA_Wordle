# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_SOURCE_DIR = /home/burton/dev/c/gba/GBA_Wordle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/burton/dev/c/gba/GBA_Wordle/build

# Include any dependencies generated for this target.
include CMakeFiles/rom_c_sources.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rom_c_sources.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rom_c_sources.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rom_c_sources.dir/flags.make

CMakeFiles/rom_c_sources.dir/src/input.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/input.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/input.c
CMakeFiles/rom_c_sources.dir/src/input.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rom_c_sources.dir/src/input.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/input.c.o -MF CMakeFiles/rom_c_sources.dir/src/input.c.o.d -o CMakeFiles/rom_c_sources.dir/src/input.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/input.c

CMakeFiles/rom_c_sources.dir/src/input.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/input.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/input.c > CMakeFiles/rom_c_sources.dir/src/input.c.i

CMakeFiles/rom_c_sources.dir/src/input.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/input.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/input.c -o CMakeFiles/rom_c_sources.dir/src/input.c.s

CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/lfsr_rand.c
CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o -MF CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o.d -o CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/lfsr_rand.c

CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/lfsr_rand.c > CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.i

CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/lfsr_rand.c -o CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.s

CMakeFiles/rom_c_sources.dir/src/main.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/main.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/main.c
CMakeFiles/rom_c_sources.dir/src/main.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/rom_c_sources.dir/src/main.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/main.c.o -MF CMakeFiles/rom_c_sources.dir/src/main.c.o.d -o CMakeFiles/rom_c_sources.dir/src/main.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/main.c

CMakeFiles/rom_c_sources.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/main.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/main.c > CMakeFiles/rom_c_sources.dir/src/main.c.i

CMakeFiles/rom_c_sources.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/main.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/main.c -o CMakeFiles/rom_c_sources.dir/src/main.c.s

CMakeFiles/rom_c_sources.dir/src/menu.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/menu.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/menu.c
CMakeFiles/rom_c_sources.dir/src/menu.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/rom_c_sources.dir/src/menu.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/menu.c.o -MF CMakeFiles/rom_c_sources.dir/src/menu.c.o.d -o CMakeFiles/rom_c_sources.dir/src/menu.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/menu.c

CMakeFiles/rom_c_sources.dir/src/menu.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/menu.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/menu.c > CMakeFiles/rom_c_sources.dir/src/menu.c.i

CMakeFiles/rom_c_sources.dir/src/menu.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/menu.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/menu.c -o CMakeFiles/rom_c_sources.dir/src/menu.c.s

CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/object_funcs.c
CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o -MF CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o.d -o CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/object_funcs.c

CMakeFiles/rom_c_sources.dir/src/object_funcs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/object_funcs.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/object_funcs.c > CMakeFiles/rom_c_sources.dir/src/object_funcs.c.i

CMakeFiles/rom_c_sources.dir/src/object_funcs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/object_funcs.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/object_funcs.c -o CMakeFiles/rom_c_sources.dir/src/object_funcs.c.s

CMakeFiles/rom_c_sources.dir/src/save_system.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/save_system.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/save_system.c
CMakeFiles/rom_c_sources.dir/src/save_system.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/rom_c_sources.dir/src/save_system.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/save_system.c.o -MF CMakeFiles/rom_c_sources.dir/src/save_system.c.o.d -o CMakeFiles/rom_c_sources.dir/src/save_system.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/save_system.c

CMakeFiles/rom_c_sources.dir/src/save_system.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/save_system.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/save_system.c > CMakeFiles/rom_c_sources.dir/src/save_system.c.i

CMakeFiles/rom_c_sources.dir/src/save_system.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/save_system.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/save_system.c -o CMakeFiles/rom_c_sources.dir/src/save_system.c.s

CMakeFiles/rom_c_sources.dir/src/subpixel.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/subpixel.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/subpixel.c
CMakeFiles/rom_c_sources.dir/src/subpixel.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/rom_c_sources.dir/src/subpixel.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/subpixel.c.o -MF CMakeFiles/rom_c_sources.dir/src/subpixel.c.o.d -o CMakeFiles/rom_c_sources.dir/src/subpixel.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/subpixel.c

CMakeFiles/rom_c_sources.dir/src/subpixel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/subpixel.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/subpixel.c > CMakeFiles/rom_c_sources.dir/src/subpixel.c.i

CMakeFiles/rom_c_sources.dir/src/subpixel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/subpixel.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/subpixel.c -o CMakeFiles/rom_c_sources.dir/src/subpixel.c.s

CMakeFiles/rom_c_sources.dir/src/txt_io.c.o: CMakeFiles/rom_c_sources.dir/flags.make
CMakeFiles/rom_c_sources.dir/src/txt_io.c.o: /home/burton/dev/c/gba/GBA_Wordle/src/txt_io.c
CMakeFiles/rom_c_sources.dir/src/txt_io.c.o: CMakeFiles/rom_c_sources.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/rom_c_sources.dir/src/txt_io.c.o"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rom_c_sources.dir/src/txt_io.c.o -MF CMakeFiles/rom_c_sources.dir/src/txt_io.c.o.d -o CMakeFiles/rom_c_sources.dir/src/txt_io.c.o -c /home/burton/dev/c/gba/GBA_Wordle/src/txt_io.c

CMakeFiles/rom_c_sources.dir/src/txt_io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rom_c_sources.dir/src/txt_io.c.i"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/burton/dev/c/gba/GBA_Wordle/src/txt_io.c > CMakeFiles/rom_c_sources.dir/src/txt_io.c.i

CMakeFiles/rom_c_sources.dir/src/txt_io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rom_c_sources.dir/src/txt_io.c.s"
	arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/burton/dev/c/gba/GBA_Wordle/src/txt_io.c -o CMakeFiles/rom_c_sources.dir/src/txt_io.c.s

rom_c_sources: CMakeFiles/rom_c_sources.dir/src/input.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/src/lfsr_rand.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/src/main.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/src/menu.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/src/object_funcs.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/src/save_system.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/src/subpixel.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/src/txt_io.c.o
rom_c_sources: CMakeFiles/rom_c_sources.dir/build.make
.PHONY : rom_c_sources

# Rule to build all files generated by this target.
CMakeFiles/rom_c_sources.dir/build: rom_c_sources
.PHONY : CMakeFiles/rom_c_sources.dir/build

CMakeFiles/rom_c_sources.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rom_c_sources.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rom_c_sources.dir/clean

CMakeFiles/rom_c_sources.dir/depend:
	cd /home/burton/dev/c/gba/GBA_Wordle/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/burton/dev/c/gba/GBA_Wordle /home/burton/dev/c/gba/GBA_Wordle /home/burton/dev/c/gba/GBA_Wordle/build /home/burton/dev/c/gba/GBA_Wordle/build /home/burton/dev/c/gba/GBA_Wordle/build/CMakeFiles/rom_c_sources.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/rom_c_sources.dir/depend

