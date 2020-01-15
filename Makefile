include ./Make.defaults

all:
	make -C boot
	make -C kernel

	tools/genfs -o root $(GENFS_MAPPINGS)
	tools/mkusr -r root \
		-k kernel/kernel.bin \
		-b boot/boot.bin \
		-o boot/fakix.img

run:
	qemu-system-$(ARCH) \
		-drive file=boot/loader.img,index=0,media=disk,format=raw \
		-drive id=fakixdisk,file=boot/fakix.img,index=1,media=disk,format=raw \
		-device ioh3420,id=root_port1,chassis=0,slot=0,bus=pcie.0 \
        -device ahci,bus=root_port1,id=ahci \
		-nographic \
		-monitor telnet::45454,server,wait \
		-serial mon:stdio \
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