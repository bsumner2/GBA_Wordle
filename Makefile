# Written by Burton Sumner with help from Tonc Library (https://www.coronac.com/tonc/text/setup.htm)


# If project name not explicitly specified during make command execution, use default name, "main."
ifeq '$(PROJ_NAME)' ''
	PROJ_NAME=$(shell pwd | sed 's-[a-zA-Z/]*/--g')
endif
ifeq '$(IP)' ''
	IP=$(shell ip route show | grep -i default | awk '{print $$3}' )
endif
#-------------------------------Project Files and Dirs-----------------------------------------------------

TARGET=$(PROJ_NAME)
ASM=./asm
SRC=./src
IWRAM_SRC=./iwsrc
BIN=./bin
INC=./include

ROM_C_OBJS=$(shell find $(SRC) -type f -iname '*.c' | sed 's-\./src-\./bin-g' | sed 's/\.c/\.o/g')
GROM_C_OBJS=$(shell find $(SRC) -type f -iname '*.c' | sed 's-\./src-\./bin-g' | sed 's/\.c/\.debug\.o/g')


ROM_CXX_OBJS=$(shell find $(SRC) -type f -iname '*.cpp' | sed 's-\./src-\./bin-g' | sed 's/\.cpp/\.o/g')
GROM_CXX_OBJS=$(shell find $(SRC) -type f -iname '*.cpp' | sed 's-\./src-\./bin-g' | sed 's/\.cpp/\.debug\.o/g')


IWRAM_C_OBJS=$(shell find $(IWRAM_SRC) -type f -iname '*.c' | sed 's-\./iwsrc-\./bin-g' | sed 's/\.c/\.o/g')
GIWRAM_C_OBJS=$(shell find $(IWRAM_SRC) -type f -iname '*.c' | sed 's-\./iwsrc-\./bin-g' | sed 's/\.c/\.debug\.o/g')

IWRAM_CXX_OBJS=$(shell find $(IWRAM_SRC) -type f -iname '*.cpp' | sed 's-\./iwsrc-\./bin-g' | sed 's/\.cpp/\.o/g')
GIWRAM_CXX_OBJS=$(shell find $(IWRAM_SRC) -type f -iname '*.cpp' | sed 's-\./iwsrc-\./bin-g' | sed 's/\.cpp/\.debug\.o/g')


C_OBJS=$(ROM_C_OBJS) $(IWRAM_C_OBJS)
GC_OBJS=$(GROM_C_OBJS) $(GIWRAM_C_OBJS)

CXX_OBJS=$(ROM_CXX_OBJS) $(IWRAM_CXX_OBJS)
GCXX_OBJS=$(GROM_CXX_OBJS) $(GIWRAM_CXX_OBJS)

S_OBJS=$(shell find $(ASM) -type f -iname '*.s' | sed 's-\./asm-\./bin-g' | sed 's/\.s/\.o/g')
GS_OBJS=$(shell find $(ASM) -type f -iname '*.s' | sed 's-\./asm-\./bin-g' | sed 's/\.s/\.debug\.o/g')

#-------------------------------Build Definitions----------------------------------------------------------

PREFIX=arm-none-eabi-
CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)g++
AS=$(PREFIX)gcc
DB=$(PREFIX)gdb
OBJ_CPY=$(PREFIX)objcopy
MGBA="/mnt/c/Program Files/mGBA/mGBA.exe"

ARCH=-mthumb-interwork -mthumb
IARCH=-mthumb-interwork -marm
SPECS=-specs=gba.specs

CFLAGS_BASE=-O2 -Wall -Wextra -fno-strict-aliasing -I$(INC) -DUSE_BIOS_VSYNC
GCFLAGS_BASE=-g -Wall -Wextra -fno-strict-aliasing -I$(INC) -DUSE_BIOS_VSYNC

GROM_CFLAGS=$(GCFLAGS_BASE) $(ARCH)
ROM_CFLAGS=$(CFLAGS_BASE) $(ARCH)

GIWRAM_CFLAGS=$(GCFLAGS_BASE) $(IARCH) -mlong-calls
IWRAM_CFLAGS=$(CFLAGS_BASE) $(IARCH) -mlong-calls

LDFLAGS=$(ARCH) $(SPECS)
GLDFLAGS=-g $(ARCH) $(SPECS)

ASFLAGS=-xassembler-with-cpp -I$(INC)
GASFLAGS=-g -xassembler-with-cpp -I$(INC)



.PHONY: build clean

#-------------------------------Build----------------------------------------------------------------------

.SILENT:

test: clean build
	$(MGBA) $(BIN)/$(TARGET).gba

debug: clean gbuild
	$(MGBA) -g $(BIN)/$(TARGET).debug.elf &
	$(DB) $(BIN)/$(TARGET).debug.elf -ex 'target remote $(IP):2345'

gbuild: $(IWRAM_SRC) $(SRC) $(BIN) $(TARGET).debug.elf

build: $(IWRAM_SRC) $(SRC) $(BIN) $(TARGET).gba

$(IWRAM_SRC) $(SRC) $(ASM) $(BIN):
	mkdir -p $@

$(TARGET).debug.elf: $(GC_OBJS) $(GCXX_OBJS) $(GS_OBJS)
	$(LD) $^ $(GLDFLAGS) -o $(BIN)/$@


$(TARGET).gba: $(TARGET).elf
	$(OBJ_CPY) -v -O binary $(BIN)/$< $(BIN)/$@
	-@gbafix $(BIN)/$@

$(TARGET).elf: $(C_OBJS) $(CXX_OBJS) $(S_OBJS)
	$(LD) $^ $(LDFLAGS) -o $(BIN)/$@

$(GROM_CXX_OBJS): $(BIN)/%.debug.o : $(SRC)/%.cpp
	$(CXX) -c $< $(GROM_CFLAGS) -o $@

$(ROM_CXX_OBJS): $(BIN)/%.o : $(SRC)/%.cpp
	$(CXX) -c $< $(ROM_CFLAGS) -o $@

$(GROM_C_OBJS): $(BIN)/%.debug.o : $(SRC)/%.c
	$(CC) -c $< $(GROM_CFLAGS) -o $@

$(ROM_C_OBJS): $(BIN)/%.o : $(SRC)/%.c
	$(CC) -c $< $(ROM_CFLAGS) -o $@

$(GS_OBJS): $(BIN)/%.debug.o : $(ASM)/%.s
	$(AS) $(GASFLAGS) -c $< -o $@

$(S_OBJS): $(BIN)/%.o : $(ASM)/%.s
	$(AS) $(ASFLAGS) -c $< -o $@

$(GIWRAM_C_OBJS): $(BIN)/%.debug.o : $(IWRAM_SRC)/%.c
	$(CC) -c $< $(GIWRAM_CFLAGS) -o $@

$(IWRAM_C_OBJS): $(BIN)/%.o : $(IWRAM_SRC)/%.c
	$(CC) -c $< $(IWRAM_CFLAGS) -o $@

$(GIWRAM_CXX_OBJS): $(BIN)/%.debug.o : $(IWRAM_SRC)/%.cpp
	$(CXX) -c $< $(GIWRAM_CFLAGS) -o $@

$(IWRAM_CXX_OBJS): $(BIN)/%.o : $(IWRAM_SRC)/%.cpp
	$(CC) -c $< $(IWRAM_CFLAGS) -o $@

#-------------------------------Build----------------------------------------------------------------------
clean: 
	@rm -fv $(BIN)/*.elf
	@rm -fv $(BIN)/*.o
	@rm -fv $(BIN)/*.gba

clean_saves:
	@rm -fv $(BIN)/*.sav
