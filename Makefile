# Ruter REST Client

# Target
BIN = ruter
LIB = 

# Directories
BINDIR = bin/
OBJDIR = obj/
SRCDIR = src/
INCDIR = $(SRCDIR)/include

# Recursive wildcard function
rwildcard = \
	$(wildcard $1$2) \
	$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Find all C files recursively and generate a list of objects
CFILES := $(call rwildcard,$(SRCDIR),*.c)
OBJ = $(subst $(SRCDIR),$(OBJDIR),$(subst .c,.o,$(CFILES)))

# Generate headers list recursively and qualify paths
HEADERS := $(call rwildcard,$(INCDIR),*.h)
BINARY = $(addprefix $(BINDIR),$(BIN))
OBJECTS = $(OBJ)

# Compiler and linker configuration
# See http://blog.httrack.com/blog/2014/03/09/what-are-your-gcc-flags/
CC = gcc
CFLAGS = \
	-fstack-protector -Wl,-z,relro -Wl,-z,now -Wformat-security \
	-Wall -Werror -Wpointer-arith -Wformat-nonliteral -Winit-self \
	-fvisibility=hidden -std=gnu99 -c -pipe -I./$(INCDIR)
LINK = gcc
LFLAGS = \
	-lm -lcurl \
	-Wl,-O1 \
	-Wl,--discard-all \
	-Wl,--no-undefined \
	-Wl,--build-id=sha1 \
	-rdynamic

# Tools
DIRGUARD = @mkdir -p $(@D)
RM = rm -rf

# Text
MODE = release
WAIT = [ .. ]
DONE = [ OK ]

# Release build (default)
all: $(BINARY)

# Debug build
debug: CFLAGS += -g
debug: MODE = debug
debug: all

# Build binary
$(BINARY) : $(OBJECTS)
	@$(DIRGUARD)
	@printf "%s Linking executable %s (%s)" "$(WAIT)" "$@" "$(MODE)"
	@$(LINK) $(OBJECTS) $(LIB) $(LFLAGS) -o $@
	@printf "\r%s\n" "$(DONE)"

# Compile source to object code
$(OBJECTS): $(OBJDIR)%.o : $(SRCDIR)%.c $(HEADERS)
	@$(DIRGUARD)
	@printf "%s Building object %s (%s)" "$(WAIT)" "$@" "$(MODE)"
	@$(CC) $(CFLAGS) $< -o $@
	@printf "\r%s\n" "$(DONE)"

# Remove generated files
clean: 
	@printf "%s Cleaning" "$(WAIT)"
	@$(RM) $(OBJDIR) $(BINDIR)
	@printf "\r%s\n" "$(DONE)"

