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
MODE = release

# Linker
LINK = gcc
LFLAGS = -lm -lcurl

# Tools
DIRGUARD = @mkdir -p $(@D)
RM = rm -rf

# Default release build
all: $(BINARY)

# Debug build
debug: CFLAGS += -g
debug: MODE = debug
debug: all

# Build binary
$(BINARY) : $(OBJECTS)
	@$(DIRGUARD)
	@printf "[    ] Linking C executable %s (%s)" "$(BINARY)" "$(MODE)"
	@$(LINK) $(OBJECTS) $(LIB) $(LFLAGS) -o $@
	@printf "\r[ OK ]\n"

# Compile source to object code
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(HEADERS)
	@$(DIRGUARD)
	@printf "[ ** ] Building C object %s (%s)" "$@" "$(MODE)"
	@$(CC) $(CFLAGS) $< -o $@
	@printf "\r[ OK ]\n"

# Remove generated files
clean: 
	@printf "[ ** ] Removing previously built files"
	@$(RM) $(OBJDIR) $(BINDIR)
	@printf "\r[ OK ]\n"

