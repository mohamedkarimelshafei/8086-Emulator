#include "instructions_flags.h"
#include "cpu.h"
#include "stack.h"

void CLC()
{
    cpu.flags &= ~0x0001;
}

void STC()
{
    cpu.flags |= 0x0001;
}

void CLI()
{
    cpu.flags &= ~0x0200;
}

void STI()
{
    cpu.flags |= 0x0200;
}

void CLD()
{
    cpu.flags &= ~0x0400;
}

void STD()
{
    cpu.flags |= 0x0400;
}

void PUSHF()
{
    Push16(cpu.flags);
}

void POPF()
{
    cpu.flags = Pop16();
}
