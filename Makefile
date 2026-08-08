APPNAME   := $(notdir $(CURDIR))
TARGETDIR := target
OBJDIR    := $(TARGETDIR)/obj
SRCDIR    := src

# Compiler and Flags
CC        ?= gcc
CFLAGS    ?= -Wall -Wextra -O2
CFLAGS    += -MMD -MP

# specify libraries here, e.g. -lm for the math library
LDLIBS    := 

# File Discovery
# We look for files in the src/ directory
SOURCES   := $(wildcard $(SRCDIR)/*.c)
# This maps src/file.c to target/obj/file.o
OBJECTS   := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
DEPFILES  := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGETDIR)/$(APPNAME)

# Link the final binary
$(TARGETDIR)/$(APPNAME): $(OBJECTS) | $(TARGETDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile src/*.c files into target/obj/*.o
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Create the directories
$(TARGETDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rvf $(TARGETDIR)

-include $(DEPFILES)
