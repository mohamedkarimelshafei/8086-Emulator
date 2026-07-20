#include "instructions_string.h"
#include "cpu.h"
#include "register8.h"
#include "flags.h"
#include <stdio.h>
void Lodsb()
{
    uint16_t seg = cpu.segmentOverride ?
                   cpu.overrideSegment :
                   cpu.ds;

    uint8_t value = Read8(seg, cpu.si);

    cpu.ax = (cpu.ax & 0xFF00) | value;

    if(cpu.flags & DF)
        cpu.si--;
    else
        cpu.si++;
cpu.segmentOverride = false;
}
void LODSW()
{
    uint16_t seg = cpu.ds;

    if(cpu.segmentOverride)
        seg = cpu.overrideSegment;
printf("LODSW %04X:%04X\n", seg, cpu.si);

    uint32_t addr = Address(seg, cpu.si);

    cpu.ax = memory[addr] | (memory[addr + 1] << 8);

    if(cpu.flags & DF)
        cpu.si -= 2;
    else
        cpu.si += 2;

    // clear after string instruction
    cpu.segmentOverride = false;
}
