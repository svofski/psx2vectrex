#!/bin/sh
DEV=/dev/tty.SLAB_USBtoUART
kill -HUP `ps ax | grep -e "[0-9] screen.*linvor" | cut -f 1 -d ' '`
./mega8_bootload vectroxnik.bin $DEV 19200
