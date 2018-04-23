/*
 * oled.c
 *
 *  Created on: 31.10.2017
 *      Author: dennis
 */

#include <avr/interrupt.h>

#include "Header/oled.h"
#include "Header/I2C.h"
#include "Header/font.h"

static uint8_t OLED_x = 0, OLED_y = 0, font_size = 0;

const uint8_t ssd1306_oled_init [] PROGMEM = {
Set_Display_Off,
Set_NOP,
Set_Lower_Column,
Set_Higher_Column,
Set_Memory_Addressing_Mode, Set_Horizontal_Adressing_Mode,
Set_Page_Adress,
Set_Multiplex_Ratio, Set_Multiplex_To,
Set_Display_Offset, Set_No_Offset,
Set_Display_Start_Line,
Set_Segment_Remap_02,
Set_COM_Output_Remapped,			//Vllt remapped machen
Set_COM_Pins, Set_COM_Pins_Hardware,
Set_VCOMH_Deselect, Set_VCOMH,
Set_Contrast_Control, Set_Contrast_1,
Entire_Display_On,
Set_Normal_Display,
Set_Display_Clock, Set_Display_Clock_01,
//Set_Precharge_Period, Set_Precharge_Period_To,
Set_Charge_Bumb_Setting, Set_Charge_Bumb_Enable,
Set_Display_On
};

void oled_init(void){
	i2c_start(OLED_ssd1306);
	i2c_write(command);
	for (uint8_t tmp = 0; tmp < sizeof(ssd1306_oled_init); tmp++)
	{
		i2c_write(pgm_read_byte(&ssd1306_oled_init[tmp]));
	}
	i2c_stop();
	clear_oled();
}

void clear_oled(void){
	i2c_start(OLED_ssd1306);
	i2c_write(data);
	for (uint16_t i = 0; i < 128<<4 ; i++) {	// 128 * (64 / Byte)
	        i2c_write(0);
	}
	i2c_stop();
	OLED_gotoxy(0,0);
}

void OLED_gotoxy (uint8_t x, uint8_t y){
	OLED_x = x;
	OLED_y = y;
    i2c_start(OLED_ssd1306);
    i2c_write(command);
    i2c_write(Set_Page_Start|y);
	i2c_write(Set_Column_Adress);
    i2c_write(x * 8);
	i2c_write(Set_Display_Start_Line|0x3F);
    i2c_stop();
}
void oled_font_size (uint8_t byte){
	font_size = byte;
}

void oled_write_char (char c){
	if(font_size != 0){
		uint8_t tmp = 0;
		OLED_gotoxy(OLED_x,OLED_y);
		i2c_start(OLED_ssd1306);
		i2c_write(data);
		for (uint8_t count = 0; count < 8; count++){
			tmp = pgm_read_byte(&font[(unsigned char)c][count]);
			tmp = tmp & 0x0f;
			tmp = ((tmp&0x01)*3)+(((tmp&0x02)<<1)*3)+(((tmp&0x04)<<2)*3)+(((tmp&0x08)<<3)*3);
			i2c_write(tmp);
			i2c_write(tmp);
		}
		i2c_stop();
		OLED_gotoxy(OLED_x,OLED_y + 1);
		i2c_start(OLED_ssd1306);
		i2c_write(data);
		for (uint8_t count = 0; count < 8; count++){
			tmp = pgm_read_byte(&font[(unsigned char)c][count]);
			tmp = (tmp & 0xf0)>>4;
			tmp = ((tmp&0x01)*3)+(((tmp&0x02)<<1)*3)+(((tmp&0x04)<<2)*3)+(((tmp&0x08)<<3)*3);
			i2c_write(tmp);
			i2c_write(tmp);
		}
		i2c_stop();
		OLED_x +=2;
		OLED_y -=1;
	}else{
		OLED_gotoxy(OLED_x,OLED_y);
		i2c_start(OLED_ssd1306);
		i2c_write(data);
		for (uint8_t count = 0; count < 8; count++){
			i2c_write(pgm_read_byte(&font[(unsigned char)c][count]));
		}
		i2c_stop();
		OLED_x +=1;
	}
}

//***************************************************************************************
void oled_write_str (char* str){
    while (*str) {
        oled_write_char(*str++);
    }
}

//***************************************************************************************
void oled_write_P (const char *Buffer,...)
{
	va_list ap;
	va_start (ap, Buffer);

	int format_flag;
	char str_buffer[10];
	char str_null_buffer[10];
	char move = 0;
	char Base = 0;
	int tmp = 0;
	char by;
	char *ptr;

	//Ausgabe der Zeichen
    for(;;){
		by = pgm_read_byte(Buffer++);
		if(by==0) break; // end of format string

		if (by == '%'){
            by = pgm_read_byte(Buffer++);
			if (isdigit(by)>0){
 				str_null_buffer[0] = by;
				str_null_buffer[1] = '\0';
				move = atoi(str_null_buffer);
                by = pgm_read_byte(Buffer++);
				}

			switch (by){
                case 's':
                    ptr = va_arg(ap,char *);
                    while(*ptr) { oled_write_char(*ptr++); }
                    break;
				case 'b':
					Base = 2;
					goto ConversionLoop;
				case 'c':
					//Int to char
					format_flag = va_arg(ap,int);
					oled_write_char (format_flag++);
					break;
				case 'i':
					Base = 10;
					goto ConversionLoop;
				case 'o':
					Base = 8;
					goto ConversionLoop;
				case 'x':
					Base = 16;
					//****************************
					ConversionLoop:
					//****************************
					itoa(va_arg(ap,int),str_buffer,Base);
					int b=0;
					while (str_buffer[b++] != 0){};
					b--;
					if (b<move){
						move -=b;
						for (tmp = 0;tmp<move;tmp++){
							str_null_buffer[tmp] = '0';
							}
						//tmp ++;
						str_null_buffer[tmp] = '\0';
						strcat(str_null_buffer,str_buffer);
						strcpy(str_buffer,str_null_buffer);
						}
					oled_write_str (str_buffer);
					move =0;
					break;
				}
		}else{
			oled_write_char (by);
		}
	}
	va_end(ap);
}


