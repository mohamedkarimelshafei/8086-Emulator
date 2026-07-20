#include "instructions_mov_mem8.h"
#include "register8.h"

void MovMem8AL()
{
    uint16_t offset = Fetch16();

    uint32_t addr =
        ((uint32_t)cpu.ds << 4) + offset;

    memory[addr] = GetReg8(0); // AL
}
