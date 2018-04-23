/*
 * I2C.c
 *
 *Diese Routine beschreibt den Ablauf eines
 *I2C Protokolls.
 *
 *  Created on: 26.10.2017
 *      Author: Dennis Pockrandt
 */
#include <avr/io.h>
#include <util/twi.h>
#include "Header/I2C.h"


//Prozessor CPU
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

//I2C Takt
#define I2C_CPU 400000UL


 void i2c_init (void){

	TWSR = 0;	//TWSR = 0 => Prescaler = 1
	TWBR = ((F_CPU/I2C_CPU)-16)/2;
}

 unsigned char i2c_start (unsigned char addr){

	//Sende START Befehle
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	/*Warten auf TWINT Flancke. Ist ein Indikator das die START
	Bedingungen gesetzt sind*/
	while (!(TWCR &(1<<TWINT)));

	/*Ueberprueft das TWI Status Register Ist der Stauts anders als der Start
	 gibt er eine 1 aus*/
	if ((TWSR & 0xF8) != TW_START && ((TWSR & 0xF8) != TW_REP_START)){
		return 1;
	}

	/*Lade die Schreibaddresse in das TWDR Register
	 * Füge das TWINT bit in das TWCR bit ein. Um die Übertragung zu starten
	 */
	TWDR = addr;
	TWCR = (1<<TWINT) | (1<<TWEN);

	/*
	 * Warte bis die Uebertragung erfolgreich war
	 */
	while (!(TWCR & (1<<TWINT)));

	/*
	Ueberpruefe den Wert des TWI Staus Register
	 */
	if ((TWSR & 0xF8)!= TW_MT_SLA_ACK && (TWSR & 0xF8) != TW_MR_SLA_ACK){
		return 1;
	}
	return 0;
 }

 void i2c_stop (void){

 	 /*
 	  * Sende Stop Bedingungen
 	  */
 	 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

 	 /*
 	  * Warten bis die Stop Bedingungen umgesetzt wurden
 	  */

 	 while (TWCR & (1<<TWSTO));


  }

 unsigned char i2c_write (unsigned char data){


	 /*
	  * sende Daten an das Geraet welches zuvor deklariert wurde
	  */

	 TWDR = data;
	 TWCR = (1<<TWINT) | (1<<TWEN);

	 /*
	  * Warte bis die Übertragung vorbei ist
	  */

	 while (!(TWCR & (1<<TWINT)));

	 /*
	  * Ueberpruefe den Wert TWI Stauts Register
	  */

	 if ((TWSR & 0xF8) != TW_MT_DATA_ACK){
		 return 1;
	 }
	 return 0;
}


 unsigned char i2c_readAck (void) {
	 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	 while (!(TWCR & (1<<TWINT)));
	 return TWDR;
 }


 unsigned char i2c_readNak (void){

	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}










