/*
 * main.c
 *
 *  Created on: 26.10.2017
 *      Author: dennis
 */


#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

#include "Header/oled.h"
#include "Header/I2C.h"
#include "Header/time.h"
#include "Header/sensor.h"
#include "Header/float2string.h"

int a =0xA0, b = 0, c = 0;

#define Licht_an PORTC |= (1<<PC0);
#define Licht_aus PORTC &= ~(1<<PC0);
#define Waerme_an PORTC |= (1<<PC1);
#define Waerme_aus PORTC &= ~(1<<PC1);
#define Wasser_an PORTC |= (1<<PC2);
#define Wasser_aus PORTC &= ~(1<<PC2);
#define Lichterkette_an PORTC |= (1<<PC3);
#define Lichterkette_aus PORTC &= ~(1<<PC3);

uint8_t BCDzuDezimal(uint8_t dez)
{
  return (dez / 16 * 10) + (dez % 16);
}

uint8_t DezimalzuBCD(uint8_t bcd)
{
  return (bcd/10*16) + (bcd%10);
}

int main (void){

	enum{Normalzustand, Minutenstellen, Stundenstellen, Waermelampe_an_min, Waermelampe_an_hours, Waermelampe_aus_min,
	        Waermelampe_aus_hours, Leuchtstoff_an_min, Leuchtstoff_an_hours, Leuchtstoff_aus_min, Leuchtstoff_aus_hours,
	        Beregnung_an_min, Beregnung_an_hours, Beregnung_aus_min, Beregnung_aus_hours,
			Lichterkette_an_min, Lichterkette_an_hours, Lichterkette_aus_min, Lichterkette_aus_hours, Schwellwert_min, Schwellwert_max};

	int state = Normalzustand;

    uint8_t sec_BCD = 0;
	uint8_t min_BCD = 0;
	uint8_t hours_BCD = 0;

	uint8_t min1_BCD = 0;
	uint8_t hours1_BCD = 0;

	uint8_t sec = 0;
	uint8_t min = 0;
	uint8_t hours = 0;

	uint8_t min1 = 0;
	uint8_t hours1 = 0;

	uint8_t w_an_min = 30;
	uint8_t w_an_hours = 10;
	uint8_t w_aus_min = 30;
	uint8_t w_aus_hours = 15;

	uint8_t l_an_min = 0;
	uint8_t l_an_hours = 9;
	uint8_t l_aus_min = 0;
	uint8_t l_aus_hours = 17;

	uint8_t b_an_min = 0;
	uint8_t b_an_hours = 10;
	uint8_t b_aus_min = 0;
	uint8_t b_aus_hours = 17;

	uint8_t k_an_min = 0;
	uint8_t k_an_hours = 19;
	uint8_t k_aus_min = 0;
	uint8_t k_aus_hours = 21;

	char Buffer2[20];
	float temp = 0;
	float feu = 0;

	float i = 65;
	float j = 75;



	i2c_init();
	oled_init();
	OLED_gotoxy(0,0);
	clear_oled();

	//Eingänge definieren
	DDRB = 0b00000000;
	PORTB = 0b00000000;

	//Ausgänge deklarieren
	DDRC = 0b00001111;

	int tasterist1 = 0;
	int tasterist2 = 0;

	while(1){

		if(tasterist1 == 0  && (PINB & (1<<PB1)))
				{
					tasterist1 = 1;
				}
		else if(tasterist1 == 1  && (PINB & (1<<PB1)))
				{
					tasterist1 = 2;
				}
		else if(tasterist1 == 2  && !(PINB & (1<<PB1)))
				{
					tasterist1 = 3;
				}
		else if(tasterist1 == 3  && !(PINB & (1<<PB1)))
				{
					tasterist1 = 0;
				}

		if(tasterist2 == 0  && (PINB & (1<<PB2)))
				{
					tasterist2 = 1;
				}
		else if(tasterist2 == 1  && (PINB & (1<<PB2)))
				{
					tasterist2 = 2;
				}
		else if(tasterist2 == 2  && !(PINB & (1<<PB2)))
				{
					tasterist2 = 3;
				}
		else if(tasterist2 == 3  && !(PINB & (1<<PB2)))
				{
					tasterist2 = 0;
				}


		switch (state){

	case Normalzustand:

			sec_BCD = sekunden();
			min_BCD = minuten();
			hours_BCD = stunden();

			sec = BCDzuDezimal(sec_BCD);
			min = BCDzuDezimal(min_BCD);
			hours = BCDzuDezimal(hours_BCD);



			OLED_gotoxy(0,0);
			oled_font_size(1);
			oled_write("%2i:%2i:%2i",hours, min, sec);
			OLED_gotoxy(0,2);
			oled_font_size(1);
			oled_write_str ("Te:");
			 temp = temperatur();
			ftoa(temp, Buffer2, 1);
			oled_write_str (Buffer2);
			oled_write_str ("%");
			OLED_gotoxy(0,4);
			oled_write_str ("Fe:");
			feu = feuchtigkeit();
			ftoa(feu, Buffer2, 1);
			oled_write_str (Buffer2);
			oled_write_str ("%");



			if (tasterist2 == 2)
			{
				state = Minutenstellen;
				OLED_gotoxy(0,0);
				clear_oled();
				Wasser_an;
			}

		    break;

	case Minutenstellen:

		        min1=min;

		       if (tasterist1 == 2 )
		        {
		            min1 = min1 + 1;
		            if (min1 > 59)
		            {
		                min1 = 0;
		            }
		        }

		        min1_BCD = DezimalzuBCD(min1);


		        i2c_start(PCF8583P + i2c_w);     // set device address and write mode
		        i2c_write(0x03);                        // write address
		        i2c_write(min1_BCD);                        // write value
		        i2c_stop();

		        min=min1;


		        OLED_gotoxy(0,0);
		        oled_font_size(0);
		        oled_write_str ("Min stellen");
		        OLED_gotoxy(0,2);
		        oled_font_size(1);
		        oled_write("%2i:%2i",hours, min, sec);





			if (tasterist2 == 1 )
			{
				state = Stundenstellen;
				OLED_gotoxy(0,0);
				clear_oled();
			}

		    break;

	case Stundenstellen:

		        hours1=hours;

		        if (tasterist1 == 2 )
		        {


					hours1 = hours1 + 1;
					if (hours1 > 23)
					{
						hours1 = 0;
					}
		        }

		        hours1_BCD = DezimalzuBCD(hours1);


		        i2c_start(PCF8583P + i2c_w);     // set device address and write mode
		        i2c_write(0x04);                        // write address
		        i2c_write(hours1_BCD);                        // write value
		        i2c_stop();

		        hours=hours1;

		        OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("h stellen");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",hours, min);

				if (tasterist2 == 2)
				{
					state = Waermelampe_an_min;
					OLED_gotoxy(0,0);
					clear_oled();
				}

		    break;

	case Waermelampe_an_min:

		    	if (tasterist1 == 2 )
		        {
		        w_an_min = w_an_min + 1;
		            if (w_an_min > 59)
		            {
		                w_an_min = 0;

		            }
		        }


		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Waerme Minuten");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",w_an_hours, w_an_min);

				if (tasterist2 == 2)
				{
					state = Waermelampe_an_hours;
					OLED_gotoxy(0,0);
					clear_oled();
				}

		    break;

		    case Waermelampe_an_hours:

		    	if (tasterist1 == 2 )
		        {
		        w_an_hours = w_an_hours + 1;
		            if (w_an_hours > 23)
		            {
		                w_an_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Waerme an h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",w_an_hours, w_an_min);

				if (tasterist2 == 1)
		        {
		            state = Waermelampe_aus_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }
		    break;

		    case Waermelampe_aus_min:

		    	if (tasterist1 == 2 )
		        {
		        w_aus_min = w_aus_min + 1;
		            if (w_aus_min > 59)
		            {
		                w_aus_min = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Waerme aus min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",w_aus_hours, w_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Waermelampe_aus_hours;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Waermelampe_aus_hours:

		    	if (tasterist1 == 2 )
		        {
		        w_aus_hours = w_aus_hours + 1;
		            if (w_aus_hours > 23)
		            {
		                w_aus_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Waerme aus h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",w_aus_hours, w_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Leuchtstoff_an_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }
		    break;

		    case Leuchtstoff_an_min:

		    	if (tasterist1 == 2 )
		        {
		        l_an_min = l_an_min + 1;
		            if (l_an_min > 59)
		            {
		                l_an_min = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Licht an min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",l_an_hours, l_an_min);

				if (tasterist2 == 2)
		        {
		            state = Leuchtstoff_an_hours;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Leuchtstoff_an_hours:

		    	if (tasterist1 == 2 )
		        {
		        l_an_hours = l_an_hours + 1;
		            if (l_an_hours > 23)
		            {
		                l_an_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Licht an h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",l_an_hours, l_an_min);

				if (tasterist2 == 2)
		        {
		            state = Leuchtstoff_aus_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Leuchtstoff_aus_min:

		    	if (tasterist1 == 2 )
		        {
		        l_aus_min = l_aus_min + 1;
		            if (l_aus_min > 59)
		            {
		                l_aus_min = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Licht aus min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",l_aus_hours, l_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Leuchtstoff_aus_hours;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Leuchtstoff_aus_hours:

		    	if (tasterist1 == 2 )
		        {
		        l_aus_hours = l_aus_hours + 1;
		            if (l_aus_hours > 23)
		            {
		                l_aus_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Licht aus h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",l_aus_hours, l_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Beregnung_an_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }
		    break;

		    case Beregnung_an_min:


		    	if (tasterist1 == 2 )
		        {
		        b_an_min = b_an_min + 1;
		            if (b_an_min > 59)
		            {
		                b_an_min = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Bereg an min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",b_an_hours, b_an_min);

				if (tasterist2 == 2)
		        {
		            state = Beregnung_an_hours;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Beregnung_an_hours:

		    	if (tasterist1 == 2 )
		        {
		        b_an_hours = b_an_hours + 1;
		            if (b_an_hours > 23)
		            {
		                b_an_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Bereg an h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",b_an_hours, b_an_min);

				if (tasterist2 == 2)
		        {
		            state = Beregnung_aus_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Beregnung_aus_min:

		    	if (tasterist1 == 2 )
		        {
		        b_aus_min = b_aus_min + 1;
		            if (b_aus_min > 59)
		            {
		                b_aus_min = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Bereg aus min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",b_aus_hours, b_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Beregnung_aus_hours;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Beregnung_aus_hours:

		    	if (tasterist1 == 2 )
		        {
		            b_aus_hours = b_aus_hours + 1;
		            if (b_aus_hours > 23)
		            {
		                b_aus_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Bereg aus h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",b_aus_hours, b_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Lichterkette_an_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }
		    break;

		    case Lichterkette_an_min:


		    	if (tasterist1 == 2 )
		        {
		        k_an_min = k_an_min + 1;
		            if (k_an_min > 59)
		            {
		                k_an_min = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Kette an min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",k_an_hours, k_an_min);

				if (tasterist2 == 2)
		        {
		            state = Lichterkette_an_hours;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Lichterkette_an_hours:

		    	if (tasterist1 == 2 )
		        {
		        k_an_hours = k_an_hours + 1;
		            if (k_an_hours > 23)
		            {
		                k_an_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Kette an h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",k_an_hours, k_an_min);

				if (tasterist2 == 2)
		        {
		            state = Lichterkette_aus_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Lichterkette_aus_min:

		    	if (tasterist1 == 2 )
		        {
		        k_aus_min = k_aus_min + 1;
		            if (k_aus_min > 59)
		            {
		                k_aus_min = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Kette aus min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",k_aus_hours, k_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Lichterkette_aus_hours;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }

		    break;

		    case Lichterkette_aus_hours:

		    	if (tasterist1 == 2 )
		        {
		            k_aus_hours = k_aus_hours + 1;
		            if (k_aus_hours > 23)
		            {
		                k_aus_hours = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Kette aus h");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				oled_write("%2i:%2i",k_aus_hours, k_aus_min);

				if (tasterist2 == 2)
		        {
		            state = Schwellwert_min;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }
		    break;


		    case Schwellwert_min:

		    	if (tasterist1 == 2 )
		        {
		            i = i + 1;
		            if (i > 99)
		            {
		                i = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Feuchtigkeit min");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				ftoa(i, Buffer2, 1);
				oled_write_str (Buffer2);
				oled_write_str ("%");

				if (tasterist2 == 2)
		        {
		            state = Schwellwert_max;
		            OLED_gotoxy(0,0);
		            clear_oled();
		        }
		    break;

		    case Schwellwert_max:

		    	if (tasterist1 == 2 )
		        {
		            j = j + 1;
		            if (j > 99)
		            {
		                j = 0;
		            }
		        }

		    	OLED_gotoxy(0,0);
				oled_font_size(0);
				oled_write_str ("Feuchtigkeit max");
				OLED_gotoxy(0,2);
				oled_font_size(1);
				ftoa(j, Buffer2, 1);
				oled_write_str (Buffer2);
				oled_write_str ("%");

				if (tasterist2 == 2)
		        {
		            state = Normalzustand;
		            clear_oled();
		        }
		    break;
		   }

		if(hours < w_an_hours)
		{
		    Waerme_aus;
		}
		else if(hours <= w_an_hours && min < w_an_min)
		{
		    Waerme_aus;
		}
		else if(hours >= w_an_hours && hours <w_aus_hours && min >= w_an_min)
		{
		    Waerme_an;
		}
		else if (hours >w_an_hours && hours <= w_aus_hours && min < w_aus_min )
		{
		    Waerme_an;
		}
		else if (hours == w_an_hours + 1 )
		{
		    Waerme_an;
		}
		else if(hours >= w_aus_hours && min >= w_aus_min)
		{
		    Waerme_aus;
		}
		else if(hours > w_aus_hours)
		{
		    Waerme_aus;
		}

		if(hours < k_an_hours)
				{
					Lichterkette_aus;
				}
				else if(hours <= k_an_hours && min < k_an_min)
				{
					Lichterkette_aus;
				}
				else if(hours >= k_an_hours && hours <k_aus_hours && min >= k_an_min)
				{
					Lichterkette_an;
				}
				else if (hours >k_an_hours && hours <= k_aus_hours && min < k_aus_min )
				{
					Lichterkette_an;
				}
				else if (hours == k_an_hours + 1 )
				{
					Lichterkette_an;
				}
				else if(hours >= k_aus_hours && min >= k_aus_min)
				{
					Lichterkette_aus;
				}
				else if(hours > k_aus_hours)
				{
				    Lichterkette_aus;
				}


		if(hours < l_an_hours)
		{
		    Licht_aus;
		}
		else if(hours <= l_an_hours && min < l_an_min)
		{
			Licht_aus;
		}
		else if(hours >= l_an_hours && hours <l_aus_hours && min >= l_an_min)
		{
			Licht_an;
		}
		else if (hours >l_an_hours && hours <= l_aus_hours && min < l_aus_min )
		{
			Licht_an;
		}
		else if (hours == l_an_hours + 1 )
		{
			Licht_an;
		}
		else if(hours >= l_aus_hours && min >= l_aus_min)
		{
			Licht_aus;
		}
		else if(hours > l_aus_hours)
		{
			Licht_aus;
		}

		if(feu <= i)
		{
		    if(hours < b_an_hours)
		    {
		        Wasser_aus;
		    }
		    else if(hours <= b_an_hours && min < b_an_min)
		    {
		    	Wasser_aus;
		    }
		    else if(hours >= b_an_hours && hours <b_aus_hours && min >= b_an_min)
		    {
		    	Wasser_an;
		    }
		    else if (hours > b_an_hours && hours <= b_aus_hours && min < b_aus_min )
		    {
		    	Wasser_an;
		    }
		    else if (hours == b_an_hours + 1 )
		{
		    	Wasser_an;
		}
		    else if(hours >= b_aus_hours && min >= b_aus_min)
		    {
		    	Wasser_aus;
		    }
		    else if(hours > b_aus_hours)
		    {
		    	Wasser_aus;
		    }
		}
		else if (feu >= j)
		{
			Wasser_aus;
		}

	}

}
