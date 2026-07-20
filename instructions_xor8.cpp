#include "instructions_xor8.h"
#include "cpu.h"
#include "register8.h"
#include "modrm8.h"

void XOR_RM8_R8()
{
    uint8_t modrm = Fetch8();

    uint8_t mod = modrm >> 6;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    uint8_t src = GetReg8(reg);

    uint8_t dst = ReadRM8(mod, rm);

    uint8_t result = dst ^ src;

    WriteRM8(mod, rm, result);

    cpu.flags &= ~(0x40 | 0x80);

    if(result == 0)
        cpu.flags |= 0x40;

    if(result & 0x80)
        cpu.flags |= 0x80;
}
void XorR8Rm8()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    uint8_t src = GetRM8(modrm);
    uint8_t dst = GetReg8(reg);

    uint8_t result = dst ^ src;

    SetReg8(reg, result);

    cpu.flags &= ~0x40;

    if(result == 0)
        cpu.flags |= 0x40;
}
