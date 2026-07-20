#include "instructions_mov_mem16.h"
#include "cpu.h"

void MovMoffs16AX()
{
    uint16_t off = Fetch16();
    uint32_t addr = Address(cpu.ds, off);

    memory[addr] = cpu.ax & 0xFF;
    memory[addr + 1] = cpu.ax >> 8;
}
