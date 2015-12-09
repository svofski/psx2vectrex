#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "ioconfig.h"

#include <stdio.h>
#include <avr/pgmspace.h>

static spi_target_t spi_target;

static uint8_t* spi_buffer;
static uint8_t  spi_index;
static uint8_t  spi_count;
static spi_callback_t spi_callback;
static spi_callback_t spi_callback_byte;
static spi_callback_t spi_enqueued;

void spi_init(void) 
{
    spi_target = SPI_TARGET_NONE;
    spi_index = 0;
    spi_count = 0;
    spi_callback = 0;
    spi_enqueued = 0;
}

int spi_acquire(spi_target_t target) {
    if (spi_target == SPI_TARGET_NONE) {
        spi_target = target;
        return target;
    }

    return SPI_TARGET_NONE;
}

void spi_acquire_wait(spi_target_t target, spi_callback_t init) {
    while(spi_acquire(target) == SPI_TARGET_NONE);
    init(0);
}

inline void spi_next() {
//    printf_P(PSTR("<%02x>"), spi_buffer[spi_index]);
    SPDR = spi_buffer[spi_index];
}

void spi_start_exchange(uint8_t *buf, uint8_t count, 
    spi_callback_t byte, spi_callback_t ready) {
    spi_buffer = buf;
    spi_index = 0;
    spi_count = count;
    spi_callback_byte = byte;
    spi_callback = ready;

    spi_next();
}

void spi_enqueue_transfer(spi_target_t target, spi_callback_t start)
{
    if (spi_acquire(target) == target) {
        start(0); 
    } 
    else if (spi_enqueued == 0) {
        spi_enqueued = start;
    } 
    else {
        while(spi_enqueued != 0);
        spi_enqueued = start;
    }
}

ISR(SPI_STC_vect)
{
    spi_buffer[spi_index] = SPDR;
    spi_index++;
    if (spi_index == spi_count) {
        if (spi_callback) {
            spi_callback(0);
            spi_callback = 0;
            spi_target = SPI_TARGET_NONE;
        }
        if (spi_enqueued) {
            spi_enqueued(0);
            spi_enqueued = 0;
        }
    }
    else {
        if (spi_callback_byte) spi_callback_byte(spi_index);
        spi_next();
    }
//    putchar('$');
}
