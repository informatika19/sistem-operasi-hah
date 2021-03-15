#!/bin/bash
./image-creator.sh > /dev/null 2>&1

./compileShell.sh
./kernel.sh

bochs -f if2230.config <<< "c" > /dev/null 2>&1