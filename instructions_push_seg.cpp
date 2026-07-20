#include "cpu.h"

void PUSH_ES()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.es);
}
