/*
 * File:   main.c
 * Author: exp10d3r
 *
 * Created on March 12, 2016, 10:27 PM
 */

// PIC18F14K50 Configuration Bit Settings
// 'C' source line config statements

#define _XTAL_FREQ 48000000
// PIC18F14K50 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection bits (No CPU System Clock divide)
#pragma config USBDIV = OFF     // USB Clock Selection bit (USB clock comes directly from the OSC1/OSC2 oscillator block; no divide)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config PLLEN = ON       // 4 X PLL Enable bit (Oscillator multiplied by 4)
#pragma config PCLKEN = ON      // Primary Clock Enable bit (Primary clock enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = NOSLP    // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only and disabled in Sleep mode (SBOREN is disabled))
#pragma config BORV = 30        // Brown-out Reset Voltage bits (VBOR set to 3.0 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config HFOFST = OFF     // HFINTOSC Fast Start-up bit (The system clock is held off until the HFINTOSC is stable.)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RA3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = OFF      // Boot Block Size Select bit (1kW boot block size)
#pragma config XINST = OFF       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protection bit (Block 0 not write-protected)
#pragma config WRT1 = OFF       // Table Write Protection bit (Block 1 not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <string.h>
#include "usb.h"
#include "usb_cdc.h"

#define LEDA_ON LATC6=1;
#define LEDA_OFF LATC6=0;
#define LEDB_ON LATC7=1;
#define LEDB_OFF LATC7=0;

#define LEDA_TOGGLE LATC6=!LATC6;
#define LEDB_TOGGLE LATC7=!LATC7;

void low_priority interrupt Low_Priority_Interrupt(void)
{

}

void high_priority interrupt High_Priority_Interrupt(void)
{
    if (USBIF)
    {
        usb_poll(); 
        USBIF=0;
        return;
    }
}

void main(void) {
    char const str[]="To be or not to be? That is the question...";
    ANSEL=0x00;
    ANSELH=0x00;
    TRISCbits.RC0=0;
    TRISCbits.RC6=0;
    TRISCbits.RC7=0;
    CM1CON0=0x00;
    CM2CON0=0x00;
    init_usb();
    init_usb_cdc();
    GIEL=1;
    GIEH=1;
    GIE=1;
    PEIE=1;
    for(;;)
    {
        if (cdc_get_state()==CDC_TX_READY)
        {
            cdc_tx_buffer(str, sizeof(str));
        }
      /*  if (i=1000)
        { 
            CDC_txbuffer(&r, 4);
            i=0;
        }
        r++;
        i++;*/
    }
    return;
}
