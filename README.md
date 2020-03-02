# Fakix

## Table of Contents
### 1. Introduction
### 2. How to Contribute
### 3. Creating an IDE

## 1: Introduction
### About Fakix
FAKIX (FAKe unIX) is a microkernel designed to implement POSIX 2017, while also
offering its own interface for interacting with the machine. It makes use of
servers (daemon processes), capabilities, and an advanced signal framework
called commands to provide the user with a exhaustive and secure interface for
their machine, while also providing simple POSIX functionality as well as its
own Standard C Library (C17 conformant).

It is worth pointing out that these are quite lofty goals, and that at the end
of the day, this is an educational project. Nonetheless, the goal remains the
same, although the most feasable endpoint is having full kernel functionality
along with partial POSIX and STDC functionality available for x86_64 hardware.

## 2: How to Contribute
Setting up Fakix is as simple as running
```shell
	git clone https://github.com/groverhood/fakix.git
	(cd fakix; bash fakixinst.sh)
```
Currently, Fakix requires QEMU 4.2.0, and expects you to have this installed
prior to any attempts to run it. There are no make rules for booting it on
hardware. Running Fakix on the latest version of QEMU is as simple as running:
```shell
	make && make run
```
To start Fakix, run this on a separate terminal:
```shell
	gdb -ex "target remote localhost:1234"
	...
	(gdb) b *<hex address of function/instruction you want to inspect>
	(gdb) c
```

## 3: Creating an IDE
### VSCode
I develop the x86_64 portion of Fakix on VSCode with the following set for
`settings.json` and `c_cpp_properties.json`:
```json
{
	"files.associations": {
		".vmdefs": "json",
		"Make.*": "makefile",
		"*.h": "c",
		"mkusr": "perl",
		"genfs": "perl",
		"*.inc": "c",
		"random": "c"
	},
	"C_Cpp.intelliSenseCacheSize": 0
}
```
```json
{
    "configurations": [
        {
            "name": "Fakix Kernel (x86_64)",
            "includePath": [
                "${workspaceFolder}/inc/libc",
                "${workspaceFolder}/inc/kernel",
                "${workspaceFolder}/inc/kernel/arch/x86_64",
                "${workspaceFolder}/inc/libfak",
                "${workspaceFolder}/inc/libfak/arch/x86_64"
            ],
            "compilerPath": "${workspaceFolder}/cross/bin/x86_64-elf-gcc",
            "intelliSenseMode": "gcc-x64",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "defines": ["ARCH_x86_64", "__FAKIX__", "__FAKIX_KERNEL__"]
        },
        {
            "name": "Fakix (x86_64)",
            "includePath": [
                "${workspaceFolder}/inc/libc",
                "${workspaceFolder}/inc/libfak",
                "${workspaceFolder}/inc/libfak/arch/x86_64"
            ],
            "compilerPath": "${workspaceFolder}/cross/bin/x86_64-elf-gcc",
            "intelliSenseMode": "gcc-x64",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "defines": ["ARCH_x86_64", "__FAKIX__"]
        }
    ],
    "version": 4
}
```
These configuration examples will always be up to date with the latest version
of Fakix.