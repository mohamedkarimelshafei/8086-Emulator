#include "instructions_sub16.h"
#include "cpu.h"
#include "modrm.h"

void SubR16Rm16()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    uint16_t src = GetRM16(modrm);

    uint16_t *dst = nullptr;

    switch(reg)
    {
        case 0: dst=&cpu.ax; break;
        case 1: dst=&cpu.cx; break;
        case 2: dst=&cpu.dx; break;
        case 3: dst=&cpu.bx; break;
        case 4: dst=&cpu.sp; break;
        case 5: dst=&cpu.bp; break;
        case 6: dst=&cpu.si; break;
        case 7: dst=&cpu.di; break;
    }

    *dst -= src;
}
