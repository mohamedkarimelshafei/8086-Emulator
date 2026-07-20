#include "interrupt.h"
#include "stack.h"

IntVector ivt[256];
void BIOSInterrupt(unsigned char number);

void Interrupt(uint8_t number)
{
if(number == 0x10 ||
       number == 0x13 ||
       number == 0x16)
    {
        BIOSInterrupt(number);
        return;
    }
    // Save FLAGS, CS, and IP
    Push16(cpu.flags);
    Push16(cpu.cs);
    Push16(cpu.ip);

    // Read the interrupt vector from the IVT
    uint32_t vector = number * 4;

    cpu.ip =
        memory[vector] |
        (memory[vector + 1] << 8);

    cpu.cs =
        memory[vector + 2] |
        (memory[vector + 3] << 8);
}

void IRET()
{
    cpu.ip = Pop16();
    cpu.cs = Pop16();
    cpu.flags = Pop16();
}
