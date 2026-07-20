#include "instructions_mov_imm8.h"
#include "register8.h"

void MovR8Imm(uint8_t reg)
{
    uint8_t value = Fetch8();
    SetReg8(reg, value);
}
