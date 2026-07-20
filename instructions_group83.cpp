#include "instructions_group83.h"
#include "cpu.h"
#include <cstdio>

extern uint16_t *reg16[8];
void Group83()
{
    uint8_t modrm = Fetch8();
    uint8_t imm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    if ((modrm >> 6) == 3)      // register operand
    {
        int16_t simm = (int8_t)imm;

        switch (reg)
        {case 0: *reg16[rm] += simm; break; // ADD
case 1: // OR
{
    *reg16[rm] |= (uint16_t)simm;

    // TODO: update flags correctly.
    // OR clears CF and OF and updates SF, ZF, PF.
    break;
}
case 4: *reg16[rm] &= (uint16_t)simm; break; // AND
//case 5: *reg16[rm] -= simm; break; // SUB
case 6: *reg16[rm] ^= (uint16_t)simm; break; // XOR
case 7:
{
    uint16_t result = *reg16[rm] - simm;
    // TODO: update flags
    (void)result;
    break;
}
            case 5: // SUB
            {
               *reg16[rm] -= simm;
                break;
            }

            default:
                printf("83 reg=%u not implemented\n", reg);
                cpu.running = false;
                break;
        }
    }
    else
    {
        printf("83 memory form not implemented\n");
        cpu.running = false;
    }
}
