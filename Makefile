# Ruter REST Client

# Target
BIN = ruter
LIB = 

# Directories
BINDIR = bin
OBJDIR = obj
SRCDIR = src
INCDIR = $(SRCDIR)/include

# Recursive wildcard function
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Find all C files recursively and generate objects list
CFILES := $(call rwildcard,$(SRCDIR),*.c)
OBJ = $(subst $(SRCDIR),$(OBJDIR),$(subst .c,.o,$(CFILES)))

# Generate headers list recursively and qualify paths
HEADERS := $(call rwildcard,$(INCDIR),*.h)
BINARY = $(addprefix $(BINDIR)/, $(BIN))
OBJECTS = $(OBJ)

# Compiler
CC = gcc
CFLAGS = -c -Wall -std=gnu99 -I./$(INCDIR)

# Linker
LINK = gcc
LFLAGS = -lm -lcurl

# Tools
DIRGUARD = @mkdir -p $(@D)
RM = rm -rf

# Text
MODE = release
WAIT = [ .. ]
DONE = [ OK ]

# Default release build
all: $(BINARY)

# Debug build
debug: CFLAGS += -g
debug: MODE = debug
debug: all

# Build binary
$(BINARY) : $(OBJECTS)
	@$(DIRGUARD)
	@printf "%s Linking C executable %s (%s)" "$(WAIT)" "$@" "$(MODE)"
	@$(LINK) $(OBJECTS) $(LIB) $(LFLAGS) -o $@
	@printf "\r%s\n" "$(DONE)"

# Compile source to object code
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(HEADERS)
	@$(DIRGUARD)
	@printf "%s Building C object %s (%s)" "$(WAIT)" "$@" "$(MODE)"
	@$(CC) $(CFLAGS) $< -o $@
	@printf "\r%s\n" "$(DONE)"

# Remove generated files
clean: 
	@printf "%s Cleaning" "$(WAIT)"
	@$(RM) $(OBJDIR) $(BINDIR)
	@printf "\r%s\n" "$(DONE)"

