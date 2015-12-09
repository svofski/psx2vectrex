///\file main.c
///\author Viacheslav Slavinsky
///
///\brief Vectroshnik main
///
/// This is the main source file. 
/// 
/// It has stuff.
///
/// \mainpage Vectroshnik: Playstation 2 controller with Vectrex
/// \section Description
/// Vectroshnik lets you play Vectrex with a fancy Playstation controller
/// \section Files
/// - main.c    main file
///
/// \section a How it works
/// Just works
/// 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "usrat.h"
#include "ioconfig.h"
#include "spi.h"
#include "digipot.h"
#include "mando.h"

/// Program main
int main() {
    uint8_t x = 0x80, y = 0x80;
    uint8_t butt;

    usart_init(F_CPU/16/19200-1);
    printf_P(PSTR("\033[2J\033[HVECTROXNIK B%s (C)SVO 2013 PRESS @\n"), BUILDNUM);

    io_init();
    digipot_init();
    mando_init();
    spi_init();

    sei();

    mando_config();

    for(;;) {
        _delay_ms(1);

        mando_poll();
        mando_get_analog(&x, &y);
        digipot_set(x, y);

        mando_get_buttons(&butt);

        //printf_P(PSTR("%02x\n"), butt);

        BUTTASS(BUTT1, butt & 1);
        BUTTASS(BUTT2, butt & 2);
        BUTTASS(BUTT3, butt & 4);
        BUTTASS(BUTT4, butt & 8);

    }
}
