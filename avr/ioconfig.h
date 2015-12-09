///\file ioconfig.h
///\brief I/O port definitions

#pragma once

#include <avr/io.h>

#define TOKENPASTE(x,y) x ## y
#define TOKENPASTE2(x,y) TOKENPASTE(x, y)

#define BITSEL(P,x) TOKENPASTE(PORT,P) = (TOKENPASTE(PORT,P) & ~_BV(P##_N)) | (x ? _BV(P##_N) : 0)

#define BITGET(P)   (TOKENPASTE(PORT,P) & _BV(P##_N))

#define MOSI        B
#define MOSI_N      3
#define SCK         B
#define MISO        B
#define MISO_N      4
#define SCK_N       5
#define POTCS       D
#define POTCS_N     2
#define PORT_MOSI   PORT##MOSI
#define PORT_SELPOT PORT##CSPOT
#define POTSEL(x)   BITSEL(POTCS,x)
#define GETPOTSEL   BITGET(POTCS)
                  
#define POTSETUP    {\
                        TOKENPASTE2(DDR,MOSI) |= _BV(MOSI_N);\
                        TOKENPASTE2(DDR,MISO) &= ~_BV(MISO_N);\
                        TOKENPASTE2(DDR,SCK) |= _BV(SCK_N);\
                        TOKENPASTE2(DDR,POTCS) |= _BV(POTCS_N);\
                        POTSEL(1);\
                    }

#define BUTT        C
#define BUTT1       _BV(2)
#define BUTT2       _BV(3)
#define BUTT3       _BV(4)
#define BUTT4       _BV(5)
#define BUTTMASK    (BUTT1|BUTT2|BUTT3|BUTT4)
#define BUTTSETUP   {\
                        TOKENPASTE2(DDR,BUTT) |= BUTTMASK;\
                        TOKENPASTE2(PORT,BUTT) &= ~BUTTMASK;\
                    }
#define BUTTON(v)   PORT##BUTT |= v;
#define BUTTOFF(v)  PORT##BUTT &= ~v;
#define BUTTASS(b,v) TOKENPASTE2(PORT,BUTT) = (TOKENPASTE2(PORT,BUTT) & ~(b)) | (v ? b : 0)

#define PS2CS       B
#define PS2CS_N     2
#define PS2MISO     D
#define PS2MISO_N   3
#define PS2MOSI     D
#define PS2MOSI_N   4

#define PS2SEL(x)   BITSEL(PS2CS,x)
#define GETPS2SEL   BITGET(PS2CS)

#define PS2SETUP    {\
                        TOKENPASTE2(DDR,PS2CS) |= _BV(PS2CS_N);\
                        PS2SEL(1);\
                    }


#if 0
#define PS2SETUP    {\
                        TOKENPASTE2(DDR,PS2MISO) &= ~_BV(PS2MISO_N);\
                        TOKENPASTE2(DDR,PS2CS) |= _BV(PS2CS_N);\
                        TOKENPASTE2(DDR,PS2MOSI) |= _BV(PS2MOSI_N);\
                        PS2SEL(0);\
                    }
#endif

void io_init();

// $Id$
