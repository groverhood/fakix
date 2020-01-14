include ../Make.defaults
include ../Make.rules
# Optional dependency
include $(ARCH)/Make.vars

# We will compile the rest of the Boot Module seperately.
LOADER_SRC = $(ARCH)_load.$(ASEXT)
SRCS = $(shell find $(ARCH) -name "*.s" -a -not -name "$(LOADER_SRC)")
OBJS = $(ARCH)/$(SRCS:.$(ASEXT)=.o)

all: boot.bin $(LOADER_SRC:.$(ASEXT)=.o)
	$(LD) $(LFLAGS) $(word 2,$^) -e $(LOADER_ENTRY)