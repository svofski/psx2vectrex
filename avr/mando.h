#pragma once

#include <inttypes.h>
#include "spi.h"

#define SPI_TARGET_MANDO (SPI_TARGET_NONE + 2)

void mando_init(void);
void mando_poll(void);
uint8_t mando_get(uint8_t n);
void mando_get_analog(uint8_t *x, uint8_t *y);
void mando_get_buttons(uint8_t* butt);
