#include "instructions_cmp16.h"
#include "cpu.h"
#include "flags.h"

void CmpAXImm16()
{
    uint16_t value = Fetch16();

    uint32_t result = (uint32_t)cpu.ax - value;

    cpu.flags &= ~(0x40 | 0x1); // clear ZF and CF

    if((uint16_t)result == 0)
        cpu.flags |= 0x40; // Zero Flag

    if(result & 0x10000)
        cpu.flags |= 0x1; // Carry Flag
}
