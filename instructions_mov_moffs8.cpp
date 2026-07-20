#include <stdio.h>
#include "cpu.h"
extern bool segmentOverride;
extern uint16_t overrideSegment;

void MOV_AL_MEM8()
{
    uint16_t offset = Fetch16();

    uint16_t seg = segmentOverride ? overrideSegment : cpu.ds;
    uint32_t addr = ((uint32_t)seg << 4) + offset;

    cpu.ax = (cpu.ax & 0xFF00) | memory[addr];

    printf("MOV AL,%04X:%04X = %02X\n",
           seg,
           offset,
           memory[addr]);
}
