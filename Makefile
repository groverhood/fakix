
all:
	make -C boot
	make -C kernel

.DEFAULT_GOAL = all