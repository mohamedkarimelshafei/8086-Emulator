#include <stdio.h>
#include "instructions_mov_mem16.h"
#include "cpu.h"

void MovAXMem16()
{
    uint16_t offset = Fetch16();

    uint32_t addr = ((uint32_t)cpu.ds << 4) + offset;

    printf("MOV AX,[%04X]\n", offset);
    printf("DS=%04X\n", cpu.ds);
    printf("[%04X] = %02X %02X\n",
           offset,
           memory[addr],
           memory[addr + 1]);

    cpu.ax = memory[addr] |
             (memory[addr + 1] << 8);
}
