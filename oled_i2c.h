/* 
 * File:   oled_i2c.h
 * Author: Exp10der
 *
 * Created on December 18, 2016, 11:01 AM
 */

#ifndef OLED_I2C_H
#define	OLED_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CMD_COMMAND      0x80      // Single command
#define CMD_CONT_CMD      0x00      // Continous command
#define CMD_DATA      0x40      // Single Data
#define CMD_CONT_DATAT    0xC0      // Continuous Data
#define CMD_NORMAL   0xA0      //
#define CMD_SETPAGE   0x22      //

        
#define  SET_LOW_COLUMN 0x00
#define  SET_HIGH_COLUMN     0x10
#define  SET_MEMORY_MODE     0x20
#define  SET_COL_ADDRESS     0x21
#define  SET_PAGE_ADDRESS    0x22
#define  RIGHT_HORIZ_SCROLL  0x26
#define  LEFT_HORIZ_SCROLL   0x27
#define  VERT_AND_RIGHT_HORIZ_SCROLL 0x29
#define  VERT_AND_LEFT_HORIZ_SCROLL  0x2A
#define  DEACTIVATE_SCROLL   0x2E
#define  ACTIVATE_SCROLL     0x2F
#define  SET_START_LINE     0x40
#define  SET_CONTRAST        0x81
#define  CHARGE_PUMP         0x8D
#define  SEG_REMAP           0xA0
#define  SET_REVERSE       0xA1
#define  SET_VERT_SCROLL_AREA 0xA3
#define  DISPLAY_ALL_ON_RESUME 0xA4
#define  DISPLAY_ALL_ON      0xA5
#define  NORMAL_DISPLAY      0xA6
#define  INVERT_DISPLAY      0xA7
#define  DISPLAY_OFF         0xAE
#define  DISPLAY_ON          0xAF
#define  COM_SCAN_INC        0xC0
#define  COM_SCAN_DEC        0xC8
#define  SET_DISPLAY_OFFSET  0xD3
#define  SET_COM_PINS        0xDA
#define  SET_VCOM_DETECT     0xDB
#define  SET_DISPLAY_CLOCK_DIV 0xD5
#define  SET_PRECHARGE       0xD9
#define  SET_MULTIPLEX       0xA8

#define  MEMORY_MODE_HORIZ  0x00
#define  MEMORY_MODE_VERT  0x01
#define  MEMORY_MODE_PAGE  0x02
 
void init_oled();
int oled_update(char buf[], int len);

#ifdef	__cplusplus
}
#endif

#endif	/* OLED_I2C_H */

