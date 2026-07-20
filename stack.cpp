#include "stack.h"

void Push16(uint16_t value)
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, value);
}

uint16_t Pop16()
{
    uint16_t value = Read16(cpu.ss, cpu.sp);
    cpu.sp += 2;
    return value;
}
