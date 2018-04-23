/*
 * oled.h
 *
 *  Created on: 31.10.2017
 *      Author: dennis
 */

#ifndef HEADER_OLED_H_
#define HEADER_OLED_H_

#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h> //enthaelt Funktionen zum beschreiben des flash Speihers

#include "I2C.h"

//Funktionen
void oled_init(void);
void oled_font_size (uint8_t byte);
void OLED_gotoxy (uint8_t x, uint8_t y);
void clear_oled(void);
void oled_write_char (char c);
void oled_write_str (char* str);
void oled_write_P (const char *Buffer,...);

#define oled_write(format, args...)   oled_write_P(PSTR(format) , ## args)

//I2C Adresse des Bildschirms
#define OLED_ssd1306 0x78

//
#define command 0x00
#define data 0x40

//Komandos des Displays
#define Entire_Display_On 	0xA4
#define Resune_to_Ram		0xA5
#define Set_Normal_Display	0xA6
#define Set_Inverse_Display	0xA7
#define Set_Display_Off		0xAE
#define Set_Display_On		0xAF
#define Set_Display_Offset 	0xD3
#define Set_No_Offset		0x00
#define Set_Display_Clock	0xD5
#define Set_Display_Clock_01 0xF0 				//Werte von 0xF0 zu 0x00
#define Set_Divide_Ratio	0xF0

#define Set_Contrast_Control 0x81
#define Set_Contrast_1 		 0xFF
#define Set_Contrast_2 		 0xAF
#define Set_Contrast_3 		 0x3F

#define Set_Memory_Addressing_Mode 0x20
#define Set_Horizontal_Adressing_Mode 0x00
#define Set_Vertical_Adressing_Mode 0x01

#define Set_Column_Adress 	0x21
#define Set_Page_Adress 	0x22
#define Set_Page_Start		0xB0
#define Set_Lower_Column	0x00
#define Set_Higher_Column	0x10
#define Set_Display_Start_Line 0x40


#define Set_Segment_Remap_01 0xA0
#define Set_Segment_Remap_02 0xA1
#define Set_Multiplex_Ratio 0xA8
#define Set_Multiplex_To 0x3F
#define Set_COM_Output_Normal 0xC0
#define Set_COM_Output_Remapped 0xC8
#define Set_COM_Pins 0xDA
#define Set_COM_Pins_Hardware 0x12

#define Set_Precharge_Period 0xD9
#define Set_Precharge_To 0x22
#define Set_VCOMH_Deselect 0xDB
#define Set_VCOMH 0x20
#define Set_NOP 0xE3
#define Set_Charge_Bumb_Setting 0x8D
#define Set_Charge_Bumb_Enable 0x14

#endif /* HEADER_OLED_H_ */
