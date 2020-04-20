/* 
 * File:   GFX.h
 * Author: Exp10der
 *
 * Created on December 18, 2016, 3:32 PM
 */

#ifndef GFX_H
#define	GFX_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
#define MAX_ITEMS 16    
    
    typedef struct LIST
    {
        uint16_t Count;
        uint16_t PosX;
        uint16_t PosY;
        uint16_t Width;
        uint16_t Height;
        int CurrentItem;
        char* items[MAX_ITEMS];
    } LIST_t;
    
void draw_pixel(int x, int y);
void draw_line(int x0, int y0, int x1, int y1);
void draw_circle(int x, int y, int radius);
void Brezenhem(int x0, int y0, int x1, int y1);
void draw_rect(int x0, int y0, int x1, int y1);
void set_text_mode(int inverted);
void set_text_font(uint8_t* new_font);

#ifdef	__cplusplus
}
#endif

#endif	/* GFX_H */

