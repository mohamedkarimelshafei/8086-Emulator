#include "instructions_mov16.h"
#include "cpu.h"
#include "modrm.h"
void MovR16Rm16()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    uint16_t value = GetRM16(modrm);

    switch(reg)
    {
        case 0: cpu.ax = value; break;
        case 1: cpu.cx = value; break;
        case 2: cpu.dx = value; break;
        case 3: cpu.bx = value; break;
        case 4: cpu.sp = value; break;
        case 5: cpu.bp = value; break;
        case 6: cpu.si = value; break;
        case 7: cpu.di = value; break;
    }
}
void MovRm16R16()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    uint16_t value;

    switch(reg)
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

    SetRM16(modrm, value);
}
void MovSPImm()
{
    cpu.sp = Fetch16();
}
