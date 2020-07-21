# @brief 
#	Builds all files a PROJECT_FOLDER as objects files and then links them 
#	together into one executable binary
#
# @file Makefile
# @author Damjan Belavic (damjan.belavic@vuka.hr)
# @copyright Copyright (c) 2020

# Expected folder structure
# PROJECT_ROOT_FOLDER
# +--- bin/ (contains compiled binary file)
# +--- include/ (can be used for manual library drops, or symbolic link can be used)
# +--- lib/
# +--- src/ (folder with all source files and folders)
# +--- Makefile

# Compiler
CC		:= gcc
# Compiler flags
CFLAGS	:= -Wall -Wextra -g -std=c99

# Compiled binary folder
BIN		:= bin
# Source folder
SRC		:= src
# Libraries include path (if using vcpkg)
INCLUDE	:= include \
	src/crc32/include \
	src/protocol/include
	
# Precompiled library folder
LIB		:= lib 
# Linker library name
LIBRARIES	:= \
# Executable binary name
EXECUTABLE	:= eltek_protocol_demo

# Magic souse
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)

CINCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
CLIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS		:= $(SOURCES:.c=.o)

# Recipes 
all: $(BIN)/$(EXECUTABLE)

.PHONY: clean
clean:
	-$(RM) $(BIN)/$(EXECUTABLE)
	-$(RM) $(OBJECTS)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CINCLUDES) $(CLIBS) -o $@ $< $(LIBRARIES)
