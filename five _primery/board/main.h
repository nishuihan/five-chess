#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>

typedef unsigned int u32_t;
typedef unsigned char u8_t;

typedef struct
{
    int w;
    int h;
    int bpp;
    void *fb_men;
}v_fb_info;
extern int fb_one_piexl(int x, int y, u32_t color);
#endif 
