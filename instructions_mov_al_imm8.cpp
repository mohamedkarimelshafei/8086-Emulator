#include "cpu.h"

void MovALImm8()
{
    uint8_t value = Fetch8();

    cpu.ax &= 0xFF00;      // Clear AL
    cpu.ax |= value;       // Set AL
}
