#include "instructions_add.h"
#include "modrm.h"

extern uint16_t *reg16[8];

void AddAXImm()
{
    uint16_t value = Fetch16();

    cpu.ax += value;

    // Flags will be implemented later
}

void AddRegReg(uint8_t modrm)
{
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    *reg16[reg] += *reg16[rm];

    // Update FLAGS later
}

void AddRegMem(uint8_t modrm)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    if(mod == 3)
    {
        AddRegReg(modrm);
        return;
    }

    uint32_t addr = GetEA(mod, rm);

    uint16_t value =
        memory[addr] |
        (memory[addr + 1] << 8);

    *reg16[reg] += value;

    // Update FLAGS later
}
