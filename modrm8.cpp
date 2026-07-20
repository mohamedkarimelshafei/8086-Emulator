#include "modrm8.h"
#include "modrm.h"
#include "register8.h"
uint8_t GetRM8(uint8_t modrm)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm = modrm & 7;

    if(mod == 3)
        return GetReg8(rm);

    uint32_t addr = GetEA(mod, rm);

    return memory[addr];
}


void SetRM8(uint8_t modrm, uint8_t value)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm = modrm & 7;

    if(mod == 3)
    {
        SetReg8(rm, value);
        return;
    }

    uint32_t addr = GetEA(mod, rm);
    memory[addr] = value;
}
uint8_t ReadRM8(uint8_t mod, uint8_t rm)
{
    if(mod == 3)
        return GetReg8(rm);

    uint32_t addr = GetEA(mod, rm);

    return memory[addr];
}

void WriteRM8(uint8_t mod, uint8_t rm, uint8_t value)
{
    if(mod == 3)
    {
        SetReg8(rm, value);
        return;
    }

    uint32_t addr = GetEA(mod, rm);

    memory[addr] = value;
}
