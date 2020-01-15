include ../Make.defaults
include ../Make.rules

CFLAGS += -D__FAKIX_KERN__
LFLAGS += -T fakix_link.generated.ld
INCS = $(PWD)/inc/kernel $(PWD)/inc/libc $(PWD)/inc/kernel/arch/x86_64
SRCS = $(shell find $(PWD)/kernel -path */arch -prune -o \( -name "*.c" -o -name "*.$(ASEXT)" \) -a -print)
SRCS += $(shell find $(PWD)/kernel/arch/$(ARCH) \( -name "*.c" -o -name "*.$(ASEXT)" \) -a -print)
OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.$(ASEXT)=.o)

all: fakix_link.generated.ld kernel.bin