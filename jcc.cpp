#include "jcc.h"
#include "cpu.h"
#include <iostream>

using namespace std;

void ExecuteJcc()
{
    uint8_t opcode = memory[Address(cpu.cs, cpu.ip - 1)];

    int8_t offset = (int8_t)Fetch8();

    bool take = false;

    switch(opcode)
    {
        case 0x72: // JC / JB
            take = cpu.flags & 0x01;
            break;

        case 0x73: // JNC / JAE
            take = !(cpu.flags & 0x01);
            break;

        case 0x74: // JZ / JE
            take = cpu.flags & 0x40;
            break;

        case 0x75: // JNZ / JNE
            take = !(cpu.flags & 0x40);
            break;

        case 0x76: // JBE
            take = (cpu.flags & 0x01) || (cpu.flags & 0x40);
            break;

        case 0x77: // JA
            take = !(cpu.flags & 0x01) && !(cpu.flags & 0x40);
            break;

        case 0x78: // JS
            take = cpu.flags & 0x80;
            break;

        case 0x79: // JNS
            take = !(cpu.flags & 0x80);
            break;
    }

    if(take)
        cpu.ip += offset;
}
