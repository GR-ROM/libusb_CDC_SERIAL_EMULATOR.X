#include "i2c.h"
#include <xc.h>

#define TRIS_SDA TRISBbits.TRISB6
#define TRIS_SCL TRISBbits.TRISB7

#define PORT_SDA PORTBbits.RB6
#define PORT_SCL PORTBbits.RB7

static void inline SET_SDA(){TRIS_SDA=1;}
static void inline CLR_SDA(){TRIS_SDA=0;PORTBbits.RB6=0;}
static void inline SET_SCL(){TRIS_SCL=1;}
static void inline CLR_SCL(){TRIS_SCL=0;PORTBbits.RB7=0;}

#define _XTAL_FREQ 48000000
#define ACK 0
#define NACK 1
#define TIMED_OUT 2
#define TIMEOUT 100
#define I2C_DELAY 2

#define WR 0x00
#define RD 0x01

void inline ___delay_us(int delay)
{
    int i;
    for (i=0;i!=delay;i++)
    {
        asm("nop");
    }
    return;
}

void init_i2c()
{
    PORT_SDA=0;
    PORT_SCL=0;
}

void start_i2c()
{
    CLR_SCL();
    __delay_us(I2C_DELAY);
    SET_SDA();
    SET_SCL();
    __delay_us(I2C_DELAY);
    CLR_SDA();
    __delay_us(I2C_DELAY);
}

void stop_i2c()
{
    CLR_SDA();
    __delay_us(I2C_DELAY);
    SET_SCL();
    __delay_us(I2C_DELAY);
    SET_SDA();
    __delay_us(I2C_DELAY);
}

uint8_t write_i2c(uint8_t b)
{
    uint8_t i;
    uint8_t ret;
    int t=0;
    CLR_SCL();
    for (i=0;i!=8;i++)
    {
        __delay_us(I2C_DELAY);
        if (b & 0x80) SET_SDA(); else CLR_SDA();
        __delay_us(I2C_DELAY);
        b=b<<1;
        SET_SCL();
        __delay_us(I2C_DELAY);
        CLR_SCL();
    }
    __delay_us(I2C_DELAY);
    SET_SDA();
    __delay_us(I2C_DELAY);
    SET_SCL();
    while(!PORT_SCL){t++; __delay_us(100); if (t==TIMEOUT) return TIMED_OUT;};
    __delay_us(I2C_DELAY);
    if (PORT_SDA==0) ret=ACK; else ret=NACK;
    __delay_us(I2C_DELAY);
    CLR_SCL();
    return ret;
}

uint8_t read_i2c(uint8_t r)
{
    uint8_t i;
    uint8_t ret=0;
    CLR_SCL();
    __delay_us(I2C_DELAY);
    SET_SDA();
    __delay_us(I2C_DELAY);
    for (i=0;i!=8;i++)
    {
        __delay_us(I2C_DELAY);
        SET_SCL();
        __delay_us(I2C_DELAY);
        ret=ret<<1;
        if (PORT_SDA==1) ret=ret | 0x1;
        __delay_us(I2C_DELAY);
        CLR_SCL();
    }
    __delay_us(I2C_DELAY);
    if (r==ACK) CLR_SDA(); else SET_SDA();
    __delay_us(I2C_DELAY);
    SET_SCL();
    __delay_us(I2C_DELAY);
    CLR_SCL();
    __delay_us(I2C_DELAY);
    return ret;
}

