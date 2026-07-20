#include "instructions_incdec16.h"
#include "cpu.h"

void IncReg16(uint8_t reg)
{
    switch(reg)
    {
        case 0: cpu.ax++; break;
        case 1: cpu.cx++; break;
        case 2: cpu.dx++; break;
        case 3: cpu.bx++; break;
        case 4: cpu.sp++; break;
        case 5: cpu.bp++; break;
        case 6: cpu.si++; break;
        case 7: cpu.di++; break;
    }
}

void DecReg16(uint8_t reg)
{
    switch(reg)
    {
        case 0: cpu.ax--; break;
        case 1: cpu.cx--; break;
        case 2: cpu.dx--; break;
        case 3: cpu.bx--; break;
        case 4: cpu.sp--; break;
        case 5: cpu.bp--; break;
        case 6: cpu.si--; break;
        case 7: cpu.di--; break;
    }
}
