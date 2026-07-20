#include "instructions_cmp.h"
#include "modrm.h"

extern uint16_t *reg16[8];

void CmpAXImm()
{
    uint16_t value = Fetch16();

    uint16_t result = cpu.ax - value;

    (void)result;

    // FLAGS later
}

void CmpRegReg(uint8_t modrm)
{
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    uint16_t result = *reg16[reg] - *reg16[rm];

    (void)result;

    // FLAGS later
}

void CmpRegMem(uint8_t modrm)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    uint16_t value;

    if(mod == 3)
    {
        value = *reg16[rm];
    }
    else
    {
        uint32_t addr = GetEA(mod, rm);

        value =
            memory[addr] |
            (memory[addr + 1] << 8);
    }

    uint16_t result = *reg16[reg] - value;

    (void)result;

    // FLAGS later
}
