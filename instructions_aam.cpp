#include "instructions_aam.h"
#include "cpu.h"

void AAM()
{
    uint8_t base = Fetch8();

    if(base == 0)
        base = 10;

    uint8_t al = cpu.ax & 0xff;

    uint8_t ah = al / base;
    uint8_t new_al = al % base;

    cpu.ax = (ah << 8) | new_al;
}
