#!/bin/bash
#
# SOURCE THIS FILE.


export PREFIX="$(pwd)/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p "$PREFIX"

(cd "$PREFIX"
curl -#L "https://ftp.gnu.org/gnu/binutils/binutils-2.33.1.tar.gz" | tar -zx
curl -#L "https://ftp.gnu.org/gnu/gcc/gcc-9.2.0/gcc-9.2.0.tar.gz" | tar -zx

mkdir build-binutils
(cd build-binutils
../binutils-2.33.1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install)

(cd gcc-9.2.0; ./contrib/download_prerequisites)

mkdir build-gcc
(cd build-gcc
../gcc-9.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc))
