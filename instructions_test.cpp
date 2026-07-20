#include "cpu.h"
#include "flags.h"
#include "opcode.h"

void TEST_AX_IMM16()
{
    uint16_t imm = Fetch16();

    uint16_t result = cpu.ax & imm;

    cpu.flags &= ~(1 | 0x40 | 0x80); // clear CF ZF SF

    if(result == 0)
        cpu.flags |= 0x40; // ZF

    if(result & 0x8000)
        cpu.flags |= 0x80; // SF
}
