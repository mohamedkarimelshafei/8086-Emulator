#include "instructions_cmp8.h"
#include "register8.h"
#include "flags_engine.h"

void CmpALImm()
{
    uint8_t value = Fetch8();

    uint8_t al = GetReg8(0);

    uint16_t result = al - value;

    UpdateSub8(al, value, result);
}
