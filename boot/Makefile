include ../Make.defaults
include ../Make.rules
include $(ARCH)/Make.vars

INCS += ../inc/kernel/arch/$(ARCH)

# We will compile the rest of the Boot Module seperately.
LOADER_SRC = $(ARCH)_load.$(ASEXT)
SRCS = $(shell find $(ARCH) \( -name "*.$(ASEXT)" \) -a -not -name "$(LOADER_SRC)")
OBJS = $(SRCS:.$(ASEXT)=.o)
OBJS = $(OBJS:.c=.o)

all: boot.bin $(ARCH)/$(ARCH)_link.generated.ld $(LOADER_SRC:.$(ASEXT)=.o)
	$(LD) $(LFLAGS) -T $(word 2,$^) -e $(LOADER_ENTRY) --oformat=binary -o loader.bin $(word 3,$^)