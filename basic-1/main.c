#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "main.h"
 v_fb_info fb_v;

void create_fb(void)
{
    int fd;
    struct fb_var_screeninfo fb_var;
    u32_t *p = NULL;

    fd = open("/dev/fb0", O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    if(ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
    {
        perror("ioctl");
        exit(1);
    }
    fb_v.w = fb_var.xres;
    fb_v.h = fb_var.yres;
    fb_v.bpp = fb_var.bits_per_pixel;
    printf("w = %d\th = %d\tbpp = %d\n", fb_v.w, fb_v.h, fb_v.bpp);
    p = mmap(NULL, fb_v.w * fb_v.h * fb_v.bpp / 8, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    *p = 0x00ff0000;
     memset(p, 0, fb_v.w*fb_v.h*fb_v.bpp/8);
    fb_v.fb_men = p;
    close(fd);
}
void scan_screen(void)
{
    int i = 0, j = 0;
    u32_t *p = NULL;
    p = fb_v.fb_men;
    for(i = 0; i < fb_v.h; i++)
    {
        for(j = 0; j < fb_v.w; j++)
        {
            p[i * fb_v.w + j] = 0x0000ff00;
        }
    }
}

int fb_one_piexl(int x, int y, u32_t color)
{
    u32_t *p = fb_v.fb_men;
    p[x + y * fb_v.w] = color;
    return 0;
}

    
int main(void)
{
    //int x = 0, y = 0;
    create_fb();    
    //scan_screen();
    fb_backgound(0, 0, fb_v.w - 1, fb_v.h - 1, 0x00996600);
    fb_backgound(STARTX, STARTY, STARTX + (V_NUM - 1) * SPACE, STARTY + (H_NUM - 1) * SPACE, 0x00ffcc33);
    fb_board(0x000000ff);
    print_color();
    mouse_cursor(512, 384);
    //save_shape(300, 300);
    //mouse_cursor(300, 300);
    //restore_shape(300, 300);
    //mouse_cursor(512, 384);
    //mouse_doing();
    client_op();
    while(1)
    {
        //mouse_doing();
    }
    return 0;   
}
