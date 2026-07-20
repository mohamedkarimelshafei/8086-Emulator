#include "instructions_mov8.h"

#include "register8.h"
#include "modrm8.h"

void MovR8RM8(uint8_t modrm)
{
    uint8_t mod = modrm >> 6;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    SetReg8(reg, ReadRM8(mod, rm));
}

void MovRM8R8(uint8_t modrm)
{
    uint8_t mod = modrm >> 6;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    WriteRM8(mod, rm, GetReg8(reg));
}
void Op88()
{
    uint8_t modrm = Fetch8();
    MovRM8R8(modrm);
}

void Op8A()
{
    uint8_t modrm = Fetch8();
    MovR8RM8(modrm);
}
