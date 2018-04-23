#include "Header/time.h"



uint8_t sekunden()
{
    uint8_t sec_BCD1 = 0;
    i2c_init();
     i2c_start(PCF8583P + i2c_w);
     i2c_write(0x02);
     i2c_start(PCF8583P + i2c_r);
        sec_BCD1 = i2c_readNak();
        i2c_stop();
   		return (sec_BCD1);
}

uint8_t minuten()
{

    uint8_t min_BCD1 = 0;
    i2c_init();
     i2c_start(PCF8583P + i2c_w);
     i2c_write(0x03);
     i2c_start(PCF8583P + i2c_r);
        min_BCD1 = i2c_readNak();
        i2c_stop();
   		return (min_BCD1);
}

uint8_t stunden()
{

    uint8_t hour_BCD1 = 0;
    i2c_init();
     i2c_start(PCF8583P + i2c_w);
     i2c_write(0x04);
     i2c_start(PCF8583P + i2c_r);
        hour_BCD1 = i2c_readNak();
        i2c_stop();
   		return (hour_BCD1);
}

