#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


void fb_board(u32_t color)
{
    int i = 0;
    for(i = 0; i < V_NUM; i++)
        fb_line(STARTX+i*SPACE, STARTY, STARTX+i*SPACE, STARTY+(H_NUM-1)*SPACE, color);
    for(i = 0; i < H_NUM; i++)
        fb_line(STARTX, STARTY+i*SPACE, STARTX+(V_NUM-1)*SPACE,STARTY+i*SPACE, color);

}
#if 0
void scan_point(int x, int y, u32_t color)
{
    int xtmp = 0, ytmp = 0;
    xtmp = (x - STARTX) % SPACE;
    ytmp = (y - STARTY) % SPACE;
    if(xtmp >= SPACE/2 && ytmp >= SPACE/2)
    {
        x = x + SPACE- xtmp;
        y = y + SPACE- ytmp;
    }
    if(xtmp < SPACE/2 && ytmp < SPACE/2)
    {
        x = x - xtmp;
        y = y - ytmp;
    }
    if(xtmp < SPACE/2 && ytmp >= SPACE/2)
    {
        x = x - xtmp;
        y = y + SPACE- ytmp;
    }
    if(xtmp > SPACE/2 && ytmp <= SPACE/2)
    {
        x = x + SPACE- xtmp;
        y = y - ytmp;
    }
    //fb_one_piexl(x, y, color);

    fb_cricle_line(x, y, 13, color);
}
#endif

int choice_color(int x, int y)
{
    if((x>=30) && (x<=70))
    {
        if((y>=40) && (y<=80))
        {
            global_color = 0x00ffffff;
            who = 1;
        }
        if((y>=120)  && (y<=160))
        {
            global_color = 0x00000000;
            who = 2;
        }
    }

    return 0;
}

int scan_point(int x, int y)
{
    int xtmp = 0, ytmp = 0;
    #if 0
    if((x<STARTX) || (x>(STARTX + (V_NUM-1)*SPACE)))
    {
        choice_color(x, y);
        return 1;
    }
    if((y<STARTY) || (y>(STARTY + (H_NUM-1)*SPACE)))
    {
        choice_color(x, y);
        return 1;
    }
    #endif
    if(board[global_x + global_y * H_NUM] != 0)
        return 0;
    xtmp = (x - STARTX) % SPACE;
    ytmp = (y - STARTY) % SPACE;
    x += ((xtmp >= SPACE/2) ? (SPACE - xtmp) : -xtmp);
    y += ((ytmp >= SPACE/2) ? (SPACE - ytmp) : -ytmp);
    fb_cricle_line(x, y, 13, global_color);

    return 0;
}

void fb_cricle(int x0, int y0, int r, u32_t color)
{
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;
    while(x <= y)
    {
        fb_one_piexl(x0 + x, y0 + y, color);
        fb_one_piexl(x0 + y, y0 + x, color);
        fb_one_piexl(x0 + x, y0 - y, color);
        fb_one_piexl(x0 - y, y0 + x, color);
        
        fb_one_piexl(x0 - x, y0 - y, color);
        fb_one_piexl(x0 - y, y0 - x, color);
        fb_one_piexl(x0 - x, y0 + y, color);
        fb_one_piexl(x0 + y, y0 - x, color);

        if(p < 0)
            p += 4 * x + 6;
        else
        {
            p += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void fb_cricle_line(int x0, int y0, int r, u32_t color)
{
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;
    while(x <= y)
    {
        fb_line(x0 + x, y0 + y, x0 - x, y0 + y, color);
        fb_line(x0 + y, y0 + x, x0 - y, y0 + x, color);
        fb_line(x0 + y, y0 - x, x0 - y, y0 - x, color);
        fb_line(x0 + x, y0 - y, x0 - x, y0 - y, color);

        if(p < 0)
            p += 4 * x + 6;
        else
        {
            p += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void fb_backgound(int x1, int y1, int x2, int y2, u32_t color)
{
   int i = x1, j = y1;
   for (j = y1; j <= y2; j++)//don't write "if ( ; j <= y2; j++)"
       for (i = x1; i <= x2; i++)
            fb_one_piexl(i, j, color);
}
   
