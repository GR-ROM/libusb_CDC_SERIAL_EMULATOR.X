#include "GFX.h"
#include "font.h"
#include "oled_i2c.h"

#define WIDTH 128
#define HEIGHT 64

#define FONT_SYMBOL_HEIGHT_OFFSET 6
#define SYM_PROPERTIES_OFFSET 8

#define TEXT_INVERTED 1

char display_buf[1024];

uint8_t* font;
uint8_t text_mode;

void set_text_mode(int inverted)
{
    if (inverted) text_mode|=TEXT_INVERTED; else text_mode&=~TEXT_INVERTED;
}

void set_text_font(uint8_t* new_font)
{
    font=new_font;
}

void draw_pixel(int x, int y)
{
    if((x<0) || (x>=WIDTH) || (y<0) || (y>=HEIGHT)) return;
    display_buf[x+(y/8)*WIDTH]|=(1<<y%8);
}

void draw_line(int x0, int y0, int x1, int y1)
{
    int x, t;
    int deltax=abs(x1-x0);
    int deltay=abs(y1-y0);
    int error=0;
    int deltaerr=deltay;
    int y=y0;
     
     if (deltay>deltax)
     {
         t=x0;
         x0=y0;
         y0=t;
         t=x1;
         x1=y1;
         y1=t;
     }
     if (x0>x1)
     {
        t=x0;
        x0=x1;
        x1=t;
        t=y1;
        y1=y0;
        y0=t; 
     }
     for (x=x0;x!=x1;x++)
     {
        draw_pixel(x,y);
        error+=deltaerr;
         if (2*error>=deltax)
         {
             y--;
             error-=deltax;
         }
     }
}

void Brezenhem(int x0, int y0, int x1, int y1) 
{
  int A, B, sign, x, y, f, signa, signb;
  A = y1 - y0;
  B = x0 - x1;
  if (abs(A)>abs(B)) sign=1; else sign=-1;
  if (A < 0) signa=-1; else signa=1;
  if (B < 0) signb=-1; else signb=1;
  f = 0;
  x = x0, y = y0;
  draw_pixel(x0, y0);
  if (sign == -1) {
    do {
      f += A*signa;
      if (f > 0) {
        f -= B*signb;
        y+=signa;
      }
      x-=signb;
      draw_pixel(x, y);
    } while (x != x1);
  } else {
    do {
      f += B*signb;
      if (f > 0) {
        f -= A*signa;
        x-=signb;
      }
      y+=signa;
      draw_pixel(x, y);
    } while (y != y1);
  }
}

void draw_rect(int x0, int y0, int x1, int y1)
{
    Brezenhem(x0, y0, x1, y0);
    Brezenhem(x1, y0, x1, y1);
    Brezenhem(x1, y1, x0, y1);
    Brezenhem(x0, y1, x0, x0);
}

void draw_text(int x, int y, char *str)
{
    unsigned int c, t, sx, sy, sym_width, sym_height, sym_offset;
    t=x;
    sx=x;
    sy=y;
    sym_height=font[FONT_SYMBOL_HEIGHT_OFFSET];
    
    while(*str)
    {
        c=(*str)-0x20;
        sym_width=font[c*4+SYM_PROPERTIES_OFFSET];
        sym_offset=font[c*4+SYM_PROPERTIES_OFFSET+1] | font[c*4+SYM_PROPERTIES_OFFSET+2]<<8;
   
        if (x+sym_width>=WIDTH)
        {
            x=t; // перенос строки
            y+=sym_height+1;
            if (y+sym_height>=HEIGHT) return;
        }
        
        for (sy=0;sy!=sym_height;sy++)
        {
            for (sx=0;sx!=sym_width;sx++)
            {
                if (font[sym_offset+(sx/8)] & 0x1<<(sx%8)) {
                  if (!text_mode & TEXT_INVERTED) draw_pixel(x+sx, y+sy);
                } else if (text_mode & TEXT_INVERTED) draw_pixel(x+sx, y+sy);
            }
            sym_offset+=sym_width/8;
            if (sym_width%8) sym_offset++;
        }
        str++;
        x+=sym_width+1;

    }
}

LIST_t* Create_List(int x, int y, int width, int height)
{
    LIST_t* result=malloc(sizeof(LIST_t));
    if (result==0) return 0;
    result->Count=0;
    result->CurrentItem=-1;
    result->PosX=x;
    result->PosY=y;
    result->Width=width;
    result->Height=height;
}

void Add_List(LIST_t* list, char* str)
{
    int i;
    if (list->Count<MAX_ITEMS)
    {
        list->items[list->Count]=str;
        list->Count++;
    }else
    {
        for (i=0;i!=MAX_ITEMS-1;i++)
        {
            list->items[i]=list->items[i+1];
        }
        list->items[MAX_ITEMS-1]=str;
    }
}

void Clear_List(LIST_t* list)
{
    int i;
    for (i=0;i!=MAX_ITEMS;i++)
    {
        list->items[i]=0x00;
    }
    list->Count=0;
}

void Scroll_List(LIST_t* list)
{
    
}

void Draw_List(LIST_t* list)
{
    int i;
    draw_rect(list->PosX, list->PosY, list->PosX+list->Width, list->PosY+list->Height);
    for (i=0;i!=list->Count;i++)
    {
        
    }
}
