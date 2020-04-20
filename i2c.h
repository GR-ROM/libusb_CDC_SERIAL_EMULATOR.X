/* 
 * File:   simple_i2c.h
 * Author: Exp10der
 *
 * Created on September 10, 2016, 7:04 PM
 */

#include <stdint.h>

#ifndef SIMPLE_I2C_H
#define	SIMPLE_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define ACK 0
#define NACK 1
   
void start_i2c();    
void stop_i2c();
uint8_t write_i2c(uint8_t b);
uint8_t read_i2c(uint8_t r);

#ifdef	__cplusplus
}
#endif

#endif	/* SIMPLE_I2C_H */


