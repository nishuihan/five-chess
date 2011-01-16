#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "main.h"


static u32_t shape[C_WIDTH * C_HEIGHT];
extern v_fb_info fb_v;
char board[C_WIDTH * C_HEIGHT];
char who = 1;
char n_x[4] = {0, 1, 1, 1,};
char n_y[4] = {1, 1, 0, -1};
u32_t global_color = 0x00ffffff;
//char color_code[2] = {0x00ffffff, 0x00000000};
static u32_t cursor_pixel[C_WIDTH * C_HEIGHT] = {
    BORD, T___, T___, T___, T___, T___, T___, T___, T___, T___,
    BORD, BORD, T___, T___, T___, T___, T___, T___, T___, T___,
    BORD, X___, BORD, T___, T___, T___, T___, T___, T___, T___,
    BORD, X___, X___, BORD, T___, T___, T___, T___, T___, T___,
    BORD, X___, X___, X___, BORD, T___, T___, T___, T___, T___,
    BORD, X___, X___, X___, X___, BORD, T___, T___, T___, T___,
    BORD, X___, X___, X___, X___, X___, BORD, T___, T___, T___,
    BORD, X___, X___, X___, X___, X___, X___, BORD, T___, T___,
    BORD, X___, X___, X___, X___, X___, X___, X___, BORD, T___,
    BORD, X___, X___, X___, X___, X___, X___, X___, X___, BORD,
    BORD, X___, X___, X___, X___, X___, BORD, BORD, BORD, BORD,
    BORD, X___, X___, BORD, X___, X___, BORD, T___, T___, T___,
    BORD, X___, BORD, T___, BORD, X___, X___, BORD, T___, T___,
    BORD, BORD, T___, T___, BORD, X___, X___, BORD, T___, T___,
    T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
    T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
    T___, T___, T___, T___, T___, T___, BORD, BORD, T___, T___
};

void save_shape(int x, int y)
{
    u32_t *p = fb_v.fb_men;
    int i = 0, j = 0;
    for( i = 0; i < C_HEIGHT; i++)
        for(j = 0; j < C_WIDTH; j++)
            shape[i * C_WIDTH + j] = p[x + j + (y + i)*fb_v.w];
}

void restore_shape(int x, int y)
{
    int i = 0, j = 0;
    for(i = 0; i < C_HEIGHT; i++)
        for( j = 0; j < C_WIDTH; j++)
            fb_one_piexl(x + j, y + i, shape[i * C_WIDTH + j]);
}

void mouse_cursor(int x, int y)
{
    int i = 0, j = 0;
    save_shape(x, y);
    for(i = 0; i < C_HEIGHT; i++)
        for(j = 0; j < C_WIDTH; j++)
        {
            if(cursor_pixel[i * C_WIDTH + j]!=T___)
                fb_one_piexl(x + j, y + i, cursor_pixel[i * C_WIDTH + j]);
        }
}

void print_color(void)
{
    fb_cricle_line(50, 60, 20, 0x00ffffff);
    fb_cricle_line(50, 140, 20, 0x00000000);
}

int chess_count(int x, int y)
{
    int i = x;
    int j = y;

    i = (i - STARTX) / SPACE;
    j = (j - STARTY) / SPACE;

    if((x - STARTX) % SPACE > (SPACE/2))
        i++;
    if((y - STARTY) % SPACE > (SPACE/2))
        j++;
    board[i + j*H_NUM] = who;

    return 0;
}

int check_five(int x, int y)
{
    char storage = 0;
    char counter = 1;
    int i = 1;
    int j = 0;
    char nx = 0, ny = 0;

    storage = board[x + y *H_NUM];
    if (storage == 0)
        return 0;
    for(j=0; j<4; j++)
    {

        counter = 1;
        nx = x;
        ny = y;
        for(i = 1; i < 5; i++)
        {
            nx += n_x[j];
            ny += n_y[j];
            if(board[nx + ny * H_NUM] == storage)
                counter++;
            else
                break;
        }
        if(counter == 5)
            return storage;
    }
    return 0;
}

int check_all(void)
{
    int i = 0, j = 0;
    for (i = 0; i < H_NUM; i++)
        for(j = 0; j < V_NUM; j++)
            if(check_five(i, j) != 0)
            {
                //printf("%d win!\n", who);
                return 1;
            }
    return 0;
}

int get_m_info(int fd, m_event *mevent)
{
    int n;
    char buff[8] = {0};

    n = read(fd, buff, 8);
    if(n <= 0)
        return 0;
    else
    {
        mevent->mode = (buff[0] & 0x07);
        mevent->dx = buff[1];
        mevent->dy = -buff[2];
        mevent->dz = buff[3];
    }
    return n;
}

int mouse_doing(void)
{
    m_event mevent;
    int fd;
    int mx = 512;
    int my = 384;
    char victory = -1;

    fd = open("/dev/input/mice", O_RDWR|O_NONBLOCK);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    print_color();
    mouse_cursor(mx, my);
    while(1)
    {
        if(get_m_info(fd, &mevent) > 0)
        {
            restore_shape(mx, my);
            mx += mevent.dx;
            my += mevent.dy;

            mx = ((mx >= 0) ? mx : 0);
            my = ((my >= 0) ? my : 0);
            if(mx >= (fb_v.w-C_WIDTH))
            {
               mx = (fb_v.w-C_WIDTH);
            }
            if(my >= (fb_v.h-C_HEIGHT))
            {
               my = (fb_v.h-C_HEIGHT);
            }

            mouse_cursor(mx, my);
            switch(mevent.mode)
            {
                case 1: 
                    restore_shape(mx, my);
                    if (scan_point(mx, my) == 0)
                    {
                        if(global_color == 0x00ffffff)
                        {
                            global_color = 0x00000000;
                            who = 1;
                        }
                        else
                        {
                            global_color = 0x00ffffff;
                            who = 2;
                        }
                        chess_count(mx, my);
                        victory = check_all();
                    }
                    mouse_cursor(mx, my);
                    break;
                case 2: break;
                case 4: break;
                default:break;
            }
            if(victory == 1)
            {
                printf("play%d won!\n", who);
                printf("contniue ? y/n\n");
                if(getchar() == 'n')
                    break;
                else
                {
                    memset((u8_t *)fb_v.fb_men, 0, fb_v.h * fb_v.w *fb_v.bpp/8);
                    memset(board, 0, H_NUM * V_NUM);
                    fb_backgound(0, 0, fb_v.w - 1, fb_v.h - 1, 0x00996600);
                    fb_backgound(STARTX, STARTY, STARTX + (V_NUM - 1) * SPACE, STARTY + (H_NUM - 1) * SPACE, 0x00ffcc33);
                    fb_board(0x000000ff);
                    print_color();
                    global_color = 0xffffffff;
                    who = 1;
                    victory = -1;
                    mx = 512;
                    my = 384;
                    mouse_cursor(mx, my);
                }
                getchar();
            }
        }
        usleep(1000);
    }
    return 0;
}


