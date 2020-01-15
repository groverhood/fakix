
all:
	make -C boot
	make -C kernel

clean:
	find . -name "*.o" -o -name "*.generated.*" -o -name "*.bin" -delete

.DEFAULT_GOAL = all