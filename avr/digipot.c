#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ioconfig.h"
#include "digipot.h"

static uint8_t buf[4];

void digipot_init() {
    // digital potentiometer lines
    POTSETUP;

    // not active
    POTSEL(1);
}

void digipot_end(uint8_t arg) {
    POTSEL(1);
    //printf_P(PSTR("potsel=%x\n"), GETPOTSEL);
}

void digipot_byte(uint8_t arg) {
    if (arg == 2) {
        POTSEL(1);
        POTSEL(0);
    }
}

void digipot_start(uint8_t arg) {
    // mad fast SPI
    SPSR = _BV(SPI2X);

    // SPIE = 1: SPI interrupt enable
    // SPE  = 1: SPI enable
    // MSTR = 1: master
    // CPOL = 0: SCK idle low
    // CPHA = 0: sample on rising edge
    // DORD = 0: MSB first
    SPCR = _BV(SPIE) |  _BV(SPE) |  _BV(MSTR);

    POTSEL(0);
    
    spi_start_exchange(buf, sizeof(buf)/sizeof(buf[0]), digipot_byte, digipot_end);
}

inline void digipot_wait() {
    while(GETPOTSEL == 0);
}

void digipot_set(uint8_t valueA, uint8_t valueB) 
{
    digipot_wait();

    buf[0] = 0x10 | 0x01; // write to device 1
    buf[1] = valueA;
    buf[2] = 0x10 | 0x02; // write to device 2
    buf[3] = valueB;
    
    spi_enqueue_transfer(SPI_TARGET_DIGIPOT, digipot_start);
}

