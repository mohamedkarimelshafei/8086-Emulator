#include "flags.h"

void SetFlag(uint16_t flag, bool value)
{
    if(value)
        cpu.flags |= flag;
    else
        cpu.flags &= ~flag;
}

bool GetFlag(uint16_t flag)
{
    return (cpu.flags & flag) != 0;
}

void UpdateZeroSign16(uint16_t value)
{
    SetFlag(ZF, value == 0);
    SetFlag(SF, (value & 0x8000) != 0);
}
