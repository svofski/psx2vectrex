#include "ioconfig.h"

void io_init() {
    // button switches: output, all 0
    BUTTSETUP;

    // PS2 controller bitt-bang SPI
    PS2SETUP;
}

//$Id$
