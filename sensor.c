/*
 * sensor.c
 *
 *  Created on: 25.11.2017
 *      Author: dennis
 */

#include "Header/sensor.h"

/*
 * Hier wird die Temperatur ausgelesen.
 */


float temperatur(void){
	uint8_t temp_msb = 0;
	uint8_t temp_lsb = 0;
	uint16_t temp_normal = 0;
	float temp = 0;

	i2c_init();

	     i2c_start(SHT21+i2c_w);
	     i2c_write(0xE3);
	     i2c_start(SHT21+i2c_r);
	     temp_msb = i2c_readNak();
	     temp_lsb = i2c_readNak();
	     i2c_stop();

	temp_normal = (temp_msb << 8)|temp_lsb;


	temp = (175.72*((float)temp_normal/65636)-46.89);

	return temp;
}

/*
 * Hier wird die Feuchtigkeit ausgelesen.
 */

float feuchtigkeit(void){
	uint8_t feu_msb = 0;
	uint8_t feu_lsb = 0;
	uint16_t feu_normal = 0;
	float feu = 0;

	i2c_init();

	     i2c_start(SHT21+i2c_w);
	     i2c_write(0xE5);
	     i2c_start(SHT21+i2c_r);
	     feu_msb = i2c_readNak();
	     feu_lsb = i2c_readNak();
	     i2c_stop();

	     feu_normal = (feu_msb << 8)|feu_lsb;


	     feu = (125*((float)feu_normal/65636)-6);

	return feu;
}
