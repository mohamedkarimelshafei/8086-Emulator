#include "modrm.h"
#include "cpu.h"
#include "flags.h"
#include <iostream>
using namespace std;
void GROUPF7()
{
    uint8_t modrm = Fetch8();

uint8_t mod = (modrm >> 6) & 3;
uint8_t reg = (modrm >> 3) & 7;
uint8_t rm  = modrm & 7;
    switch(reg)
    {case 2:    // NOT r/m16
{
    if (mod == 3)
    {
        *reg16[rm] = ~(*reg16[rm]);
    }
    else
    {
        uint32_t ea = GetEA(mod, rm);

        uint16_t value = memory[ea] |
                         (memory[ea + 1] << 8);

        value = ~value;

        memory[ea] = value & 0xFF;
        memory[ea + 1] = value >> 8;
    }
    break;
}
case 4: // MUL r/m16
{
    uint16_t value = GetRM16(modrm);

    uint32_t result = (uint32_t)cpu.ax * value;

    cpu.ax = result & 0xFFFF;
    cpu.dx = (result >> 16) & 0xFFFF;

    if(cpu.dx != 0)
    {
        cpu.flags |= 1;   // CF
        cpu.flags |= 0x800; // OF if your flags use bit 11
    }
    else
    {
        cpu.flags &= ~1;
        cpu.flags &= ~0x800;
    }

    break;
}
        case 0: // TEST r/m16, imm16
        {
uint16_t value = GetRM16(modrm);
            uint16_t imm = Fetch16();

            uint16_t result = value & imm;


            break;
        }

        default:
            cout << "F7 group /" << (int)reg << " not implemented\n";
            cpu.running=false;
    }
}
