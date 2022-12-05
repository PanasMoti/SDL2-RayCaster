#project name
PROJECT_NAME = OhGodPleaseHelpMe

#directories
BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib

#cpp compiler
CC = g++
# compiler flags
COMPILER_FLAGS = -std=c++14 -w -I$(INC_DIR)

WIN64_SDL2 = $(LIB_DIR)/SDL2-w64

LINKER_FLAGS = -lSDL2 -lSDL2_image

WIN_LINK = -L"$(WIN64_SDL2)/lib" -I"$(WIN64_SDL2)/include"

# .cpp files
SRC_FILES = $(SRC_DIR)/*.cpp

EXE_NAME = $(PROJECT_NAME).o
WIN_EXE_NAME = $(PROJECT_NAME).exe

all:help

lbuild:
	$(CC) $(SRC_FILES) $(COMPILER_FLAGS) -o3 $(LINKER_FLAGS) -o $(BIN_DIR)/$(EXE_NAME)
lrun:
	$(BIN_DIR)/$(EXE_NAME)

ldebug:
	$(CC) $(SRC_FILES) $(COMPILER_FLAGS) -o0 $(LINKER_FLAGS) -o $(BIN_DIR)/debug$(EXE_NAME)
	$(BIN_DIR)/debug$(EXE_NAME)
	rm $(BIN_DIR)/debug$(EXE_NAME)

lclean:
	rm $(BIN_DIR)/*.o

wbuild:
	$(CC) $(SRC_FILES) $(COMPILER_FLAGS) -o3 $(WIN_LINK) $(LINKER_FLAGS) -o $(BIN_DIR)/$(WIN_EXE_NAME)
wdebug:
	$(CC) $(SRC_FILES) $(COMPILER_FLAGS) -o0 $(WIN_LINK) $(LINKER_FLAGS) -o $(BIN_DIR)/debug$(WIN_EXE_NAME)
	$(BIN_DIR)/debug$(WIN_EXE_NAME)
	del $(BIN_DIR)\debug$(WIN_EXE_NAME)
wrun:
	$(BIN_DIR)/$(WIN_EXE_NAME)

wclean:
	del $(BIN_DIR)\*.exe


help:
	$(info linux   commands: lbuild, 	ldebug,		lrun,		lclean)
	$(info windows commands: wbuild,	wdebug,		wrun,		wclean)
	@echo --------------------------------------