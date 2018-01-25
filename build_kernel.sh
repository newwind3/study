#!/bin/bash

toolchain_version=4.8
export PATH=${TOP}/prebuilts/gcc/linux-x86/arm/arm-eabi-${toolchain_version}/bin/:$PATH

echo $PATH
arm-eabi-gcc -v
