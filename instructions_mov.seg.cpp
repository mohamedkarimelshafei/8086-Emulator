#include "instructions_mov_seg.h"
#include "cpu.h"
#include "modrm.h"

void MovRm16Seg()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    uint16_t value = 0;

    switch(reg)
    {
        case 0: value = cpu.es; break;
        case 1: value = cpu.cs; break;
        case 2: value = cpu.ss; break;
        case 3: value = cpu.ds; break;
        default:
            cpu.running = false;
            return;
    }

    // For now only handle register/memory through your existing ModRM
    SetRM16(modrm, value);
}
