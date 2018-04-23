#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <avr/io.h>

#include "I2C.h"


#define PCF8583P 0xA0



uint8_t sekunden();
uint8_t minuten();
uint8_t stunden();

#endif // TIME_H_INCLUDED


