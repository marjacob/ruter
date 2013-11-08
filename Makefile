# Ruter REST Client

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
DIRGUARD=@mkdir -p $(@D)
RM=rm -rf

# Default release build
all: $(BINARY)

# Debug build
debug: CFLAGS += -g
debug: all

# Build binary
$(BINARY) : $(OBJECTS)
	$(DIRGUARD)
	$(LINK) $(OBJECTS) $(LIB) $(LFLAGS) -o $@

# Compile source to object code
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(HEADERS)
	$(DIRGUARD)
	$(CC) $(CFLAGS) $< -o $@

# Remove generated files
clean: 
	$(RM) $(OBJECTS) $(BINARY)

