/*
 * sensor.h
 *
 *  Created on: 25.11.2017
 *      Author: dennis
 */

#ifndef HEADER_SENSOR_H_
#define HEADER_SENSOR_H_

#include <avr/io.h>

#include "I2C.h"

#define SHT21 0b10000000	//Adresse wurde bereits um ein Bit verschoben


float temperatur(void);
float feuchtigkeit(void);


#endif /* HEADER_SENSOR_H_ */
