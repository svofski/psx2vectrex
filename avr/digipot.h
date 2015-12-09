#pragma once

#include <inttypes.h>

#include "spi.h"

#define SPI_TARGET_DIGIPOT  (SPI_TARGET_NONE + 1)

void digipot_init(void);

void digipot_set(uint8_t valueA, uint8_t valueB);
