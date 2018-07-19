#ifndef INC_MAIN_H
#define INC_MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define BIT(x) (1 << (x))
#define SETBITS(x, y) ((x) |= (y))
#define SETBIT(x, y) SETBITS((x), (BIT((y))))
#define CLEARBITS(x, y) ((x) &= (~(y)))
#define CLEARBIT(x, y) CLEARBITS((x), (BIT((y))))

#endif