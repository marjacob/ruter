# KMS Server Emulator

# Target
BIN=ruter
LIB=

# Directories
BINDIR=bin
OBJDIR=obj
SRCDIR=src
INCDIR=$(SRCDIR)/include

# Generate objects list
OBJ=$(subst $(SRCDIR),$(OBJDIR),$(subst .c,.o,$(wildcard $(SRCDIR)/*.c)))

# Generate headers list and qualify paths
HEADERS=$(wildcard $(INCDIR)/*.h)
BINARY=$(addprefix $(BINDIR)/, $(BIN))
OBJECTS=$(OBJ)

# Compiler
CC=gcc
CFLAGS=-c -Wall -std=gnu99 -I./$(INCDIR)

# Linker
LINK=gcc
LFLAGS=-lm -lcurl

# Tools
MKDIR=mkdir -p

# Ensure that "directories" is not associated with a file.
.PHONY: directories

# Default release build
all: directories $(BINARY)

# Debug build
debug: CFLAGS += -g
debug: all

# Create required directories.
directories: $(BINDIR) $(OBJDIR)

$(BINDIR):
	$(MKDIR) $(BINDIR)

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

# Build binary
$(BINARY) : $(OBJECTS)
	$(LINK) $(OBJECTS) $(LIB) $(LFLAGS) -o $@

# Compile source to object code
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

# Remove generated files
clean : 
	rm -rf $(OBJECTS) $(BINARY)

