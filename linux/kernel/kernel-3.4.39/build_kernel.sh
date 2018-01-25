#!/bin/bash

toolchain_version=4.8
export PATH=../../../prebuilts/gcc/linux-x86/arm/arm-eabi-${toolchain_version}/bin/:$PATH

echo $PATH
arm-eabi-gcc -v

make ARCH=arm activo_ct10_es_defconfig
make ARCH=arm uImage -j16