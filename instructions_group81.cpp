#include "instructions_group81.h"
#include "cpu.h"
#include "modrm.h"
#include <stdio.h>
void Group81()
{
    uint8_t modrm = Fetch8();
    printf("81 ModRM=%02X\n", modrm);

    uint8_t reg = (modrm >> 3) & 7;
uint16_t value = GetRM16(modrm);
uint16_t imm = Fetch16();
//    uint16_t value = GetRM16(modrm);
 //   uint16_t imm   = Fetch16();
printf("reg=%d value=%04X imm=%04X IP=%04X\n",
       reg, value, imm, cpu.ip);
    switch(reg)
    {
        case 0: // ADD
            value += imm;
            
SetRM16(modrm, value);
            break;
case 4: // AND r/m16, imm16
{
    value &= imm;
    SetRM16(modrm, value);

    // clear CF and OF
    cpu.flags &= ~(0x0001 | 0x0800);

    // update ZF
    if (value == 0)
        cpu.flags |= 0x0040;
    else
        cpu.flags &= ~0x0040;

    // update SF
    if (value & 0x8000)
        cpu.flags |= 0x0080;
    else
        cpu.flags &= ~0x0080;

    break;
}
        case 5: // SUB
            value -= imm;
            SetRM16(modrm, value);
            break;

        case 6: // XOR r/m16, imm16
{
    value ^= imm;
    SetRM16(modrm, value);
    break;
}
        case 7: // CMP
            cpu.flags = (value == imm) ? 0x40 : 0;
            break;

        default:
            printf("81 /%d not implemented\n", reg);
            cpu.running = false;
            break;
    }
}
