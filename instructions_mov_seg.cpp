#include "instructions_mov_seg.h"
#include "cpu.h"
#include "modrm.h"
uint16_t GetRM16x(uint8_t modrm)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm  = modrm & 7;

    if(mod == 3)
    {
        switch(rm)
        {
            case 0: return cpu.ax;
            case 1: return cpu.cx;
            case 2: return cpu.dx;
            case 3: return cpu.bx;
            case 4: return cpu.sp;
            case 5: return cpu.bp;
            case 6: return cpu.si;
            case 7: return cpu.di;
        }
    }

    uint32_t addr = GetEA(mod, rm);

    return memory[addr] | (memory[addr+1] << 8);
}
void MovSegRm16()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    uint16_t value = GetRM16(modrm);

    switch(reg)
    {
        case 0:
            cpu.es = value;
            break;

        case 2:
            cpu.ss = value;
            break;

        case 3:
            cpu.ds = value;
            break;

        default:
            cpu.running = false;
            break;
    }
}
