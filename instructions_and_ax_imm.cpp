#include "cpu.h"

void AndAXImm16()
{
    uint16_t imm = Fetch16();

    cpu.ax &= imm;

    // Clear Carry and Overflow
    cpu.flags &= ~(0x0001 | 0x0800);

    // Zero Flag
    if (cpu.ax == 0)
        cpu.flags |= 0x0040;
    else
        cpu.flags &= ~0x0040;

    // Sign Flag
    if (cpu.ax & 0x8000)
        cpu.flags |= 0x0080;
    else
        cpu.flags &= ~0x0080;

    // TODO: update Parity Flag (PF)
}
