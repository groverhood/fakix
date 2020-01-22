include ./Make.defaults

all:
	make -C boot
	make -C kernel

	tools/genfs -o root $(GENFS_MAPPINGS)
	tools/mkusr -r root \
		-k kernel/kernel.bin \
		-b boot/boot.bin \
		-l boot/loader.img \
		-o boot/fakix.img

format:
	find boot kernel libc \( -name "*.h" -o -name "*.c" \) | xargs clang-format -i

# -device ioh3420,id=root_port1,chassis=0,slot=0,bus=pcie.0 \
# -device ahci,bus=root_port1,id=ahci \

run:
	qemu-system-$(ARCH) \
		-drive id=fakixdisk,file=boot/fakix.img,index=0,media=disk,format=raw \
		-nographic \
		-s -S \
		-monitor telnet::45454,server,nowait \
		-serial mon:stdio \
		-m 1G \
		-M q35

clean:
	find boot kernel libc \( \
	    -name "*.o" \
	    -o -name "*.generated.*" \
	    -o -name "*.bin" \
	    -o -name "*.img" \
	\) -a -delete
	rm -rf root
	rm -f fakix.img