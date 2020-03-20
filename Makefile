include ./Make.defaults
include ./Make.rules

all: tools headers
	make -C kernel
	make -C lib
	make -C sbin

	objcopy -O binary kernel/kernel.bin boot/image/sbin/boot
	grub-mkrescue /usr/lib/grub/i386-pc -o test.img boot/image
	
tools:
	guild compile tools/generror.scm

headers:
	tools/generror.scm errors/errors.scm > include/fakix/errors.h

format:
	find boot kernel lib \( -name "*.h" -o -name "*.c" \) | xargs clang-format -i

# -device ioh3420,id=root_port1,chassis=0,slot=0,bus=pcie.0 \
# -device ahci,bus=root_port1,id=ahci \

run:
	qemu-system-$(ARCH) \
		-cdrom test.img\
		-nographic \
		-s -S \
		-monitor telnet::45454,server,nowait \
		-serial mon:stdio \
		-m 1G \
		-M q35

clean:
	find boot kernel lib sbin \( \
	    -name "*.o" \
	    -o -name "*.generated.*" \
	    -o -name "*.bin" \
	    -o -name "*.img" \
	\) -a -delete
	rm -rf boot/image/sbin/*
	touch boot/image/sbin/.keep