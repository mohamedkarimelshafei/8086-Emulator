#include "instructions_incdec8.h"
#include "modrm8.h"
#include "cpu.h"
#include <stdio.h>

void IncRM8(uint8_t modrm)
{
    uint8_t mod = modrm >> 6;
    uint8_t rm  = modrm & 7;

    uint8_t value = ReadRM8(mod, rm);

    value++;

    SetRM8(modrm, value);
}

void DecRM8(uint8_t modrm)
{
    uint8_t mod = modrm >> 6;
    uint8_t rm  = modrm & 7;

    uint8_t value = ReadRM8(mod, rm);

    value--;

    SetRM8(modrm, value);
}

void IncDec8()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    switch(reg)
    {
        case 0:
            IncRM8(modrm);
            break;

        case 1:
            DecRM8(modrm);
            break;

        default:
            printf("FE group /%d not implemented\n", reg);
            cpu.running = false;
            break;
    }
}
