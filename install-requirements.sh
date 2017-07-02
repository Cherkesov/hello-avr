#!/usr/bin/env bash

sudo apt-get install -y gcc-avr binutils-avr avr-libc
sudo apt-get install -y gdb-avr
sudo apt-get install -y avrdude

sudo ln -s /usr/lib/avr/include/avr /usr/local/include