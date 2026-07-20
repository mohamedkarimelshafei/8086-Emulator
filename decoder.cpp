#include "cpu.h"
#include <iostream>

using namespace std;

// Decode the effective address from the r/m field.
// For now, this supports register mode and the 8 basic memory forms.
// Displacements will be added later.
uint32_t EffectiveAddress(uint8_t rm)
{
    switch (rm)
    {
        case 0: return Address(cpu.ds, cpu.bx + cpu.si);
        case 1: return Address(cpu.ds, cpu.bx + cpu.di);
        case 2: return Address(cpu.ss, cpu.bp + cpu.si);
        case 3: return Address(cpu.ss, cpu.bp + cpu.di);
        case 4: return Address(cpu.ds, cpu.si);
        case 5: return Address(cpu.ds, cpu.di);
        case 6: return Address(cpu.ss, cpu.bp);
        case 7: return Address(cpu.ds, cpu.bx);
    }

    return 0;
}

void Execute()
{
    uint8_t opcode = Fetch8();

    switch(opcode)
    {
        case 0x90:      // NOP
            break;

        case 0xF4:      // HLT
            cpu.running = false;
            break;

        case 0xB8: cpu.ax = Fetch16(); break;
        case 0xB9: cpu.cx = Fetch16(); break;
        case 0xBA: cpu.dx = Fetch16(); break;
        case 0xBB: cpu.bx = Fetch16(); break;
        case 0xBC: cpu.sp = Fetch16(); break;
        case 0xBD: cpu.bp = Fetch16(); break;
        case 0xBE: cpu.si = Fetch16(); break;
        case 0xBF: cpu.di = Fetch16(); break;

        // MOV r16,r/m16
        case 0x8B:
        {
            uint8_t modrm = Fetch8();

            uint8_t mod = (modrm >> 6) & 3;
            uint8_t reg = (modrm >> 3) & 7;
            uint8_t rm  = modrm & 7;

            if(mod == 3)
            {
                // Register -> Register
                *reg16[reg] = *reg16[rm];
            }
            else
            {
                // Memory -> Register
                uint32_t addr = EffectiveAddress(rm);

                *reg16[reg] =
                    memory[addr] |
                    (memory[addr + 1] << 8);
            }

            break;
        }
case 0x03:
{
    uint8_t modrm = Fetch8();

    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    uint16_t value;

    if(mod == 3)
    {
        value = *reg16[rm];
    }
    else
    {
        uint32_t addr = EffectiveAddress(rm);

        value =
            memory[addr] |
            (memory[addr + 1] << 8);
    }

    uint16_t result = *reg16[reg] + value;

    *reg16[reg] = result;

    // minimal flags
    cpu.flags &= ~(0x40 | 0x80);

    if(result == 0)
        cpu.flags |= 0x40;   // ZF

    if(result & 0x8000)
        cpu.flags |= 0x80;   // SF

    break;
}
        default:
            cout << "Unknown opcode 0x"
                 << hex << (int)opcode << endl;
            cpu.running = false;
            break;
    }
}
