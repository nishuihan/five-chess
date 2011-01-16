
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "main.h"

extern void fb_line(int x1, int y1,int x2, int y2, u32_t color);
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
    *p = 0x0000ff00;
    fb_v.fb_men = p;
    close(fd);
}
/*
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
}*/

int fb_one_piexl(int x, int y, u32_t color)
{
    u32_t *p = fb_v.fb_men;
    p[x + y * fb_v.w] = color;
    return 0;
}

    
int main(void)
{
    create_fb();    
    fb_line(1000, 700, 0, 0, 0x00ff0000);
    fb_line(0, 400, 200, 0, 0x00ff0000);
    //scan_screen();
    return 0;   

}


