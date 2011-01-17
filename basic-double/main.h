#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>

#define V_NUM 30 
#define H_NUM 24 
#define STARTX 100
#define STARTY 40
#define SPACE 30 

#define C_WIDTH 10
#define C_HEIGHT 17

#define T___ 0x0
#define X___ 0xffff
#define BORD 0x000000ff

#define BUFFER_SIZE 3

typedef unsigned int u32_t;
typedef unsigned char u8_t;

extern int fb_one_piexl(int x, int y, u32_t color);
extern void fb_line(int x1, int y1,int x2, int y2, u32_t color);
extern void fb_board(u32_t);
extern int scan_point(int x, int y);
extern void fb_cricle(int x0, int y0, int r, u32_t color);
extern void fb_cricle_line(int x0, int y0, int r, u32_t color);
extern void fb_backgound(int x1, int y1, int x2, int y2, u32_t color);

extern void save_shape(int x, int y);
extern void restore_shape(int x, int y);
extern void mouse_cursor(int x, int y);

extern int mouse_doing(void);
extern int chess_count(int x, int y);
extern int check_five(int x, int y);
extern int check_all(void);
extern void print_color(void);
extern char board[C_WIDTH * C_HEIGHT];

extern u32_t global_color;
extern char who;

extern int global_x;
extern int global_y;
extern int fd;
extern char flag;

extern int client_op(void);

typedef struct
{
    int w;
    int h;
    int bpp;
    void *fb_men;
}v_fb_info;

typedef struct
{
    int mode;
    int dx;
    int dy;
     
    int dz;
}m_event;

typedef struct
{
    char dest;
    char source;
    char type;
    char len;
    int x;
    int y;
}message_t;


/*extern v_fb_info fb_v;*/
#endif 
