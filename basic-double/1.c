#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "main.h"

v_fb_info fb_v;

void create_fb(void)
{
    int fd, i = 0;
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
    for(i = 0; i < 1000; i++)
    {
        p = mmap(NULL, fb_v.w * fb_v.h * fb_v.bpp / 8, PROT_WRITE|PROT_READ, MAP_SHARED, fd, i);
        if(p == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }
        *p = 0x0000ff00;
    }
    close(fd);
}
int main(void)
{
    create_fb();    
    return 0;   

}


