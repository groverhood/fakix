include $(PWD)/Make.defaults
include $(PWD)/Make.rules
include $(ARCH)/Make.vars

INCS = $(PWD)/inc/kernel/arch/$(ARCH)

# We will compile the rest of the Boot Module seperately.
LOADER_SRC = $(ARCH)_load.$(ASEXT)
SRCS = $(shell find $(shell pwd)/$(ARCH) \( -name "*.$(ASEXT)" \) -a -not -name "$(LOADER_SRC)" -a -print)
OBJS := $(SRCS:.$(ASEXT)=.o)
LFLAGS += -T $(ARCH)/$(ARCH)_link.generated.ld

all: $(ARCH)/$(ARCH)_link.generated.ld boot.bin loader.bin	

loader.bin: $(ARCH)/$(LOADER_SRC:.$(ASEXT)=.o)
	$(LD) $(LFLAGS) -o $@ $(ARCH)/$(LOADER_SRC:.$(ASEXT)=.o)
