#include "instructions_xor16.h"
#include "cpu.h"

void Xor16()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    uint16_t value = 0;
    uint16_t dest = 0;

    switch(rm)
    {
        case 0: value = cpu.ax; break;
        case 1: value = cpu.cx; break;
        case 2: value = cpu.dx; break;
        case 3: value = cpu.bx; break;
        case 4: value = cpu.sp; break;
        case 5: value = cpu.bp; break;
        case 6: value = cpu.si; break;
        case 7: value = cpu.di; break;
    }

    switch(reg)
    {
        case 0: dest = cpu.ax; break;
        case 1: dest = cpu.cx; break;
        case 2: dest = cpu.dx; break;
        case 3: dest = cpu.bx; break;
        case 4: dest = cpu.sp; break;
        case 5: dest = cpu.bp; break;
        case 6: dest = cpu.si; break;
        case 7: dest = cpu.di; break;
    }

    uint16_t result = dest ^ value;

    switch(reg)
    {
        case 0: cpu.ax = result; break;
        case 1: cpu.cx = result; break;
        case 2: cpu.dx = result; break;
        case 3: cpu.bx = result; break;
        case 4: cpu.sp = result; break;
        case 5: cpu.bp = result; break;
        case 6: cpu.si = result; break;
        case 7: cpu.di = result; break;
    }
}
