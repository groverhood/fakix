include ./Make.defaults



all:
	make -C boot
	make -C kernel

clean:
	find kernel boot \( -name "*.o" -o -name "*.generated.*" -o -name "*.bin" \) -a -delete

.DEFAULT_GOAL = all