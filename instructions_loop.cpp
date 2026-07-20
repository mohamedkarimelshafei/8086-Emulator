#include <stdio.h>
#include "cpu.h"
#include "instructions_loop.h"

void LoopNZ()
{
    int8_t offset = (int8_t)Fetch8();

    printf("LOOPNZ CX=%04X FLAGS=%04X OFFSET=%d IP=%04X\n",
           cpu.cx, cpu.flags, offset, cpu.ip);

    cpu.cx--;

    if(cpu.cx != 0 && !(cpu.flags & 0x40))
    {
        cpu.ip += offset;

        printf("JUMP TO IP=%04X\n", cpu.ip);
    }
}
