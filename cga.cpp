#include "cga.h"
#include "cpu.h"
#include <stdio.h>

#define CGA_SEG 0xB800

uint8_t cga_mode = 3;

uint32_t CGAAddress(uint16_t offset)
{
    return ((uint32_t)CGA_SEG << 4) + offset;
}


void CGA_WritePort(uint16_t port, uint8_t value)
{
    printf("CGA port %04X = %02X\n", port, value);

    // CRTC ports
    // 3D8 = mode control register
    if(port == 0x3D8)
    {
        cga_mode = value;
    }
}


uint8_t CGA_ReadPort(uint16_t port)
{
    return 0;
}


void CGA_Render()
{
    // CGA text mode 80x25

    for(int y=0;y<25;y++)
    {
        for(int x=0;x<80;x++)
        {
            uint16_t off = y*160 + x*2;

            uint32_t addr = CGAAddress(off);

            uint8_t ch = memory[addr];

            if(ch == 0)
                ch=' ';

  //          printf("%c", ch);
        }

//        printf("\n");
    }
}
