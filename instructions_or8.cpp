#include "instructions_or8.h"
#include "cpu.h"
#include "register8.h"
#include "flags.h"
void Or8()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    uint8_t value = GetReg8(rm);
    uint8_t dest  = GetReg8(reg);

    uint8_t result = dest | value;

    SetReg8(reg, result);

    // OR clears CF and OF
    cpu.flags &= ~(ZF | SF | PF | CF | OF);

    if (result == 0)
        cpu.flags |= ZF;

    if (result & 0x80)
        cpu.flags |= SF;

    if (__builtin_parity(result) == 0)
        cpu.flags |= PF;
}
