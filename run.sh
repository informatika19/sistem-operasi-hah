#!/bin/bash
./image-creator.sh > /dev/null

./kernel.sh

bochs -f if2230.config <<< "c" > /dev/null 2>&1