#include "instructions_sub.h"
#include "modrm.h"

extern uint16_t *reg16[8];

void SubAXImm()
{
    uint16_t value = Fetch16();

    cpu.ax -= value;

    // FLAGS later
}

void SubRegReg(uint8_t modrm)
{
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    *reg16[reg] -= *reg16[rm];

    // FLAGS later
}

void SubRegMem(uint8_t modrm)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    if(mod == 3)
    {
        SubRegReg(modrm);
        return;
    }

    uint32_t addr = GetEA(mod, rm);

    uint16_t value =
        memory[addr] |
        (memory[addr + 1] << 8);

    *reg16[reg] -= value;

    // FLAGS later
}

