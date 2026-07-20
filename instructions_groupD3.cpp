#include "instructions_groupD3.h"
#include "cpu.h"
#include <iostream>

void GroupD3()
{
    uint8_t modrm = Fetch8();

    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    uint8_t count = cpu.cx & 0xff; // CL

    if(mod == 3)
    {
        uint16_t value = *reg16[rm];

        switch(reg)
        {
            case 4: // SHL/SAL
                value <<= count;
                break;

            case 5: // SHR
                value >>= count;
                break;

            case 7: // SAR
                value = (int16_t)value >> count;
                break;

            default:
                std::cout << "D3 group /" 
                          << (int)reg 
                          << " not implemented\n";
                return;
        }

        *reg16[rm] = value;
    }
}
