#include "register8.h"

uint8_t GetReg8(uint8_t reg)
{
    switch(reg)
    {
        case 0: return cpu.ax & 0xFF;          // AL
        case 1: return cpu.cx & 0xFF;          // CL
        case 2: return cpu.dx & 0xFF;          // DL
        case 3: return cpu.bx & 0xFF;          // BL

        case 4: return cpu.ax >> 8;            // AH
        case 5: return cpu.cx >> 8;            // CH
        case 6: return cpu.dx >> 8;            // DH
        case 7: return cpu.bx >> 8;            // BH
    }

    return 0;
}

void SetReg8(uint8_t reg, uint8_t value)
{
    switch(reg)
    {
        case 0:
            cpu.ax = (cpu.ax & 0xFF00) | value;
            break;

        case 1:
            cpu.cx = (cpu.cx & 0xFF00) | value;
            break;

        case 2:
            cpu.dx = (cpu.dx & 0xFF00) | value;
            break;

        case 3:
            cpu.bx = (cpu.bx & 0xFF00) | value;
            break;

        case 4:
            cpu.ax = (cpu.ax & 0x00FF) | (value << 8);
            break;

        case 5:
            cpu.cx = (cpu.cx & 0x00FF) | (value << 8);
            break;

        case 6:
            cpu.dx = (cpu.dx & 0x00FF) | (value << 8);
            break;

        case 7:
            cpu.bx = (cpu.bx & 0x00FF) | (value << 8);
            break;
    }
}
