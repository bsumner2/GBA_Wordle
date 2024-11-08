
# Minimum CMake version and project setup
cmake_minimum_required(VERSION 3.10)
project(GBA_Homebrew_Project C CXX ASM)

# Define directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(ASM_DIR ${CMAKE_SOURCE_DIR}/asm)
set(IWRAM_SRC_DIR ${CMAKE_SOURCE_DIR}/iwsrc)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

# Specify ARM toolchain
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(OBJ_COPY arm-none-eabi-objcopy)

# Architecture and flags
set(ARCH_FLAGS -mthumb-interwork -mthumb)
set(IARCH_FLAGS -mthumb-interwork -marm)
set(SPECS -specs=gba.specs)
set(CFLAGS_BASE -O3 -Wall -Wextra -fno-strict-aliasing -I${INCLUDE_DIR})
set(ROM_CFLAGS ${CFLAGS_BASE} ${ARCH_FLAGS})
set(IWRAM_CFLAGS ${CFLAGS_BASE} ${IARCH_FLAGS} -mlong-calls)
set(ASFLAGS -xassembler-with-cpp -I${INCLUDE_DIR})
set(LDFLAGS ${ARCH_FLAGS} ${SPECS})

# Create output directory if it doesn't exist
file(MAKE_DIRECTORY ${BIN_DIR})

# Gather source files dynamically
file(GLOB_RECURSE ROM_C_SOURCES ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE IWRAM_SOURCES ${IWRAM_SRC_DIR}/*.c ${IWRAM_SRC_DIR}/*.cpp)
file(GLOB_RECURSE ASM_SOURCES ${ASM_DIR}/*.s)

# Add libraries only if sources exist
if(ROM_C_SOURCES)
    add_library(rom_c_sources OBJECT ${ROM_C_SOURCES})
    target_compile_options(rom_c_sources PRIVATE ${ROM_CFLAGS})
    list(APPEND PROJECT_LIBRARIES rom_c_sources)
endif()

if(IWRAM_SOURCES)
    add_library(iwram_c_sources OBJECT ${IWRAM_SOURCES})
    target_compile_options(iwram_c_sources PRIVATE ${IWRAM_CFLAGS})
    list(APPEND PROJECT_LIBRARIES iwram_c_sources)
endif()

if(ASM_SOURCES)
    add_library(asm_sources OBJECT ${ASM_SOURCES})
    target_compile_options(asm_sources PRIVATE ${ASFLAGS})
    list(APPEND PROJECT_LIBRARIES asm_sources)
endif()

# Add executable target
add_executable(${PROJECT_NAME}.elf)

# Link all available libraries to the executable
target_link_libraries(${PROJECT_NAME}.elf ${PROJECT_LIBRARIES})

# Set linker flags on the executable target
target_link_options(${PROJECT_NAME}.elf PRIVATE ${LDFLAGS})

# Post-build step to generate .gba file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${OBJ_COPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
    COMMAND gbafix ${PROJECT_NAME}.gba
    COMMENT Generating GBA binary
)

# Test target with mgba
add_custom_target(test
    COMMAND mgba-qt ${PROJECT_NAME}.gba
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT Running in mGBA
)

# Clean target
add_custom_target(cleanup COMMAND rm -f ${BIN_DIR}/*.elf ${BIN_DIR}/*.o ${BIN_DIR}/*.gba
    COMMENT Cleaning build files
)
