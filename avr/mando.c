#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>
#include <string.h>
#include "ioconfig.h"
#include "spi.h"
#include "mando.h"

static uint8_t cmd_length;
static uint8_t buf[31];

void mando_init() {
    PS2SETUP;
    PS2SEL(1);
}

void mando_end(uint8_t arg) {
    _delay_us(20);
    PS2SEL(1);
}

void mando_byte(uint8_t arg) {
    _delay_us(20);
}

void mando_start(uint8_t arg) {
    // mad slow SPI
    SPSR = 0;

    // SPIE = 1: SPI interrupt enable
    // SPE  = 1: SPI enable
    // MSTR = 1: master
    // CPOL = 1: SCK idle high
    // CPHA = 0: sample on rising edge
    // DORD = 1: LSB first
    SPCR = _BV(SPIE) |  _BV(SPE) |  _BV(MSTR) | _BV(DORD) 
         | _BV(CPOL)
         | _BV(SPR1)
     //    | _BV(SPR0)
         ;
    
    _delay_us(25);
    PS2SEL(0);
    _delay_us(25);

    spi_start_exchange(buf, cmd_length, mando_byte, mando_end);
}

inline void mando_wait() {
    while(GETPS2SEL == 0);
}

uint8_t mando_get(uint8_t n) {
    return buf[n];
}

void dump() 
{
    for(int i = 0; i < 9 /*sizeof(buf)*/; i++) {
        printf_P(PSTR("%02x "), mando_get(i));
    }
    putchar('\n');
}

void mando_poll()
{
    mando_wait();

    memset(buf, 0, sizeof(buf));
    buf[0] = 0x01;
    buf[1] = 0x42;
    cmd_length = 9;
    
    spi_enqueue_transfer(SPI_TARGET_MANDO, mando_start);
    mando_wait();
    //dump();
}

static uint8_t dummy[] = {0x01};

static uint8_t poll_main[] = {0x01, 0x42, 0x00, 0x00, 0x00};
static uint8_t enter_config[]={0x01,0x43,0x00,0x01,0x00};
static uint8_t type_read[]={0x01,0x45,0x00,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t type_read46_0[]={0x01,0x46,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t type_read46_1[]={0x01,0x46,0x00,0x01,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t type_read47_0[]={0x01,0x47,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t type_read4c_0[]={0x01,0x4c,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t type_read4c_1[]={0x01,0x4c,0x00,0x01,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t check_response[]={0x01,0x41,0x00,0x5a,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t exit_config[]={0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t set_mode[]={0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};

void mando_send(const uint8_t* bytes, uint8_t len) 
{
    mando_wait();
    memset(buf, 0x5a, sizeof(buf));
    memcpy(buf, bytes, len);
    cmd_length = len;
    spi_enqueue_transfer(SPI_TARGET_MANDO, mando_start);
    mando_wait();
    _delay_ms(5);
    dump();
}

void mando_config()
{

    mando_send(poll_main, 5);
    mando_send(enter_config, 5);
    //mando_send(type_read, 9);
    //mando_send(type_read46_0, 9);
    //mando_send(type_read46_1, 9);
    //mando_send(type_read47_0, 9);
    //mando_send(type_read4c_0, 9);
    //mando_send(type_read4c_1, 9);
    //mando_send(check_response, 9);
    mando_send(set_mode, 9);
    mando_send(exit_config, 9);
    return;
}

void mando_get_analog(uint8_t *x, uint8_t *y) 
{
    if (buf[1] == 0x73) {
        uint8_t up = buf[3] & 0x10; 
        uint8_t lt = buf[3] & 0x80;
        uint8_t rt = buf[3] & 0x20;
        uint8_t dn = buf[3] & 0x40;

        *x = buf[7];
        *y = 255 - buf[8];

        if (!up) *y = 0xff;
        if (!dn) *y = 0;
        if (!lt) *x = 0;
        if (!rt) *x = 0xff;
    } else {
        *x = *y = 0x80;
    }
}

void mando_get_buttons(uint8_t* butt)
{
    *butt = 0;

    if (buf[1] == 0x73) {
        uint8_t b1 = buf[4] & 0x80;
        uint8_t b2 = buf[4] & 0x40;
        uint8_t b3 = buf[4] & 0x20;
        uint8_t b4 = buf[4] & 0x10;

        *butt = (b1 ? 0 : 1) |
                (b2 ? 0 : 2) |
                (b3 ? 0 : 4) |
                (b4 ? 0 : 8);
    }
}
