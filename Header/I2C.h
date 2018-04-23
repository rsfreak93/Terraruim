/*
 * I2C.h
 *
 *  Created on: 26.10.2017
 *      Author: dennis
 */

#ifndef HEADER_I2C_H_
#define HEADER_I2C_H_

#define i2c_w 0
#define i2c_r 1


void i2c_init (void);

unsigned char i2c_start (unsigned char addr);

void i2c_start_wait(unsigned char address);

unsigned char i2c_rep_start(unsigned char address);

void i2c_stop (void);

unsigned char i2c_write (unsigned char data);

unsigned char i2c_readAck (void);

unsigned char i2c_readNak (void);

#endif /* HEADER_I2C_H_ */

