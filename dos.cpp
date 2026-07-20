#include "dos.h"
#include "cpu.h"

void DOSInterrupt()
{
    switch(cpu.ax >> 8)
    {
        case 0x4C:
            cpu.running = false;
            break;

        default:
            break;
    }
}
