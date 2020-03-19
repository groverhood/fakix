include ../Make.defaults
include ../Make.rules

CFLAGS += -D__FAKIX_KERN__ -fPIE
LFLAGS += -T fakix_link.generated.ld --no-relax
INCS = $(PWD)/kernel/include $(PWD)/include 
INCS += $(PWD)/kernel/include/arch/$(ARCH) $(PWD)/include/arch/$(ARCH)
SRCS = $(shell find $(PWD)/kernel -path "*/arch" -prune -o \( -name "*.c" -o -name "*.$(ASEXT)" \) -a -print)
SRCS += $(shell find $(PWD)/kernel/arch/$(ARCH) \( -name "*.c" -o -name "*.$(ASEXT)" \) -a -print)
OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.$(ASEXT)=.o)

all: fakix_link.generated.ld kernel.bin