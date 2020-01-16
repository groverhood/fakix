include $(PWD)/Make.defaults
include $(PWD)/Make.rules
include $(ARCH)/Make.vars

INCS = $(PWD)/inc/kernel/arch/$(ARCH)

# We will compile the rest of the Boot Module seperately.
LOADER_SRC = $(ARCH)_load.$(ASEXT)
SRCS = $(shell find $(shell pwd)/$(ARCH) \( -name "*.$(ASEXT)" \) -a -not -name "$(LOADER_SRC)" -a -print)
OBJS := $(SRCS:.$(ASEXT)=.o)
BOOTLINK = $(ARCH)/$(ARCH)_link.generated.ld
LFLAGS += -T $(BOOTLINK)
OBC = $(CROSS)/$(PREFIX)-objcopy

all: $(BOOTLINK) boot.bin loader.bin
	$(OBC) $(OBCFLAGS) loader.bin loader2.bin

boot.bin: LFLAGS += -e real_mode_entry

loader.bin: OBJS := $(ARCH)/$(ARCH)_load.o
loader.bin: LFLAGS += -e load_entry