#include "kernel.h"
#include "../cpu/cpu.h"

void DOSKernel::Int21()
{
    switch(cpu.AH)
    {
        case 0x00:
            Int20();
            break;

        case 0x01:
            break;

        case 0x02:
            break;

        case 0x09:
            break;

        case 0x0F:
            break;

        default:
            break;
    }
}
