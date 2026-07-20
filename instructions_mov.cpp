#include "instructions_mov.h"
#include "modrm.h"
#include <iostream>

using namespace std;

extern uint16_t *reg16[8];

void MovRegReg(uint8_t modrm)
{
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    *reg16[reg] = *reg16[rm];
}

void MovRegMem(uint8_t modrm)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    if(mod == 3)
    {
        MovRegReg(modrm);
        return;
    }

    uint32_t addr = GetEA(mod, rm);

    uint16_t value =
        memory[addr] |
        (memory[addr + 1] << 8);

    *reg16[reg] = value;
}

void MovMemReg(uint8_t modrm)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    if(mod == 3)
    {
        *reg16[rm] = *reg16[reg];
        return;
    }

    uint32_t addr = GetEA(mod, rm);

    uint16_t value = *reg16[reg];

    memory[addr]     = value & 0xFF;
    memory[addr + 1] = value >> 8;
}
