
#include "fbtool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG
#ifdef DEBUG

int main()

{

    FBDEV fbdev;

    memset(&fbdev, 0, sizeof(FBDEV));

    strcpy(fbdev.dev, "/dev/fb0");

    if(fb_open(&fbdev)==FALSE)

    {

        printf("open frame buffer error ");

        return;

    }

//注意，下面一行有bug

    unsigned long temp;

    temp= fbdev.fb_mem+fbdev.fb_mem_offset;

    fb_memset((void *)temp, 0, fbdev.fb_fix.smem_len);

    fb_close(&fbdev);

}

#endif
