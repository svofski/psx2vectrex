#pragma once

#include <inttypes.h>

#define SPI_TARGET_NONE 0

typedef uint8_t spi_target_t;

typedef void (*spi_callback_t)(uint8_t arg);

void spi_init(void);

int spi_acquire(spi_target_t target);
void spi_acquire_wait(spi_target_t target, spi_callback_t init);
void spi_start_exchange(uint8_t* buf, uint8_t count, 
    spi_callback_t byte, spi_callback_t ready);
void spi_enqueue_transfer(spi_target_t target, spi_callback_t start);

// $Id$
