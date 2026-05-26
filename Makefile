.PHONY: all clean

# --- CONFIGURATION ---
PROJECT_NAME       = MazeVisualizator
EXT = .exe
RAYLIB_PATH        ?= ..\..
COMPILER_PATH      ?= C:/raylib/w64devkit/bin
PLATFORM           ?= PLATFORM_DESKTOP

# Raylib locations
DESTDIR ?= /usr/local
RAYLIB_INSTALL_PATH ?= $(DESTDIR)/lib
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include

# Compiler settings
CC = g++
MAKE = mingw32-make

# Compiler flags
CFLAGS += -Wall -std=c++17 -D_DEFAULT_SOURCE -Wno-missing-braces
CFLAGS += -g -O0 -static #Debug build

# Windows specific settings
ifeq ($(OS),Windows_NT)
    PLATFORM_OS=WINDOWS
    export PATH := $(COMPILER_PATH):$(PATH)
    # Subsystem windows hides the console. Remove if you want console output.
    CFLAGS += -Wl,--subsystem,windows
    LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lcomdlg32 -lole32
endif

# Include and Library paths
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external
LDFLAGS = -L. -L$(RAYLIB_PATH)/src

# --- FILE FINDING LOGIC ---
# Recursive wildcard function to find all files in subfolders
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Automatically find ALL .cpp files in src folder and subfolders
SRC = $(call rwildcard, src, *.cpp)

# EXPLICITLY GRAB C/C++ LIBRARIES FROM THE LIB FOLDER
LIB_SRC = lib/tinyfiledialogs.c

# --- BUILD TARGETS ---

all:
	@echo Compiling the entire project...
	
	@echo 1. Compiling icon to temporary resource...
	windres icon/icon.rc -o temp_icon.o -I icon
	
	@echo 2. Compiling sources and linking executable...
	$(CC) -o $(PROJECT_NAME)$(EXT) $(SRC) $(LIB_SRC) temp_icon.o $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)
	
	@echo 3. Cleaning temporary icon file...
	del temp_icon.o
	
	@echo Build complete! Created $(PROJECT_NAME)$(EXT)

# Clean target
clean:
	del *.exe
	@echo Cleaned up executables.