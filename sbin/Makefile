include ../Make.defaults
include ../Make.rules

INCS = $(PWD)/include $(PWD)/include/arch/$(ARCH)

all: sbin_link.generated.ld
	make -C init
	cp init/init.bin $(PWD)/boot/image/sbin/init