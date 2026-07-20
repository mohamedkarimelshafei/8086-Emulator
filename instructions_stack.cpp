#include "instructions_stack.h"
#include "stack.h"

extern uint16_t *reg16[8];

void PushReg(uint8_t reg)
{
    Push16(*reg16[reg]);
}

void PopReg(uint8_t reg)
{
    *reg16[reg] = Pop16();
}
