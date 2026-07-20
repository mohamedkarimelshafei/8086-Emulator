#include "flags_engine.h"
#include "flags.h"

bool Parity(uint8_t value)
{
    bool p = true;

    while(value)
    {
        if(value & 1)
            p = !p;

        value >>= 1;
    }

    return p;
}

void UpdateAdd16(uint16_t a,uint16_t b,uint32_t result)
{
    SetFlag(CF,result > 0xFFFF);

    SetFlag(ZF,(uint16_t)result == 0);

    SetFlag(SF,result & 0x8000);

    SetFlag(PF,Parity(result));

    SetFlag(AF,((a ^ b ^ result) & 0x10));

    SetFlag(OF,
        (~(a ^ b) &
        (a ^ result) &
        0x8000));
}

void UpdateSub16(uint16_t a,uint16_t b,uint32_t result)
{
    SetFlag(CF,a < b);

    SetFlag(ZF,(uint16_t)result == 0);

    SetFlag(SF,result & 0x8000);

    SetFlag(PF,Parity(result));

    SetFlag(AF,((a ^ b ^ result) & 0x10));

    SetFlag(OF,
        ((a ^ b) &
        (a ^ result) &
        0x8000));
}

void UpdateAdd8(uint8_t a,uint8_t b,uint16_t result)
{
    SetFlag(CF,result > 0xFF);

    SetFlag(ZF,(uint8_t)result == 0);

    SetFlag(SF,result & 0x80);

    SetFlag(PF,Parity(result));

    SetFlag(AF,((a ^ b ^ result) & 0x10));

    SetFlag(OF,
        (~(a ^ b) &
        (a ^ result) &
        0x80));
}

void UpdateSub8(uint8_t a,uint8_t b,uint16_t result)
{
    SetFlag(CF,a < b);

    SetFlag(ZF,(uint8_t)result == 0);

    SetFlag(SF,result & 0x80);

    SetFlag(PF,Parity(result));

    SetFlag(AF,((a ^ b ^ result) & 0x10));

    SetFlag(OF,
        ((a ^ b) &
        (a ^ result) &
        0x80));
}
