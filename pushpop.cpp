#include "instructions_stack.h"

void PushAX()
{
    PushReg(0);
}

void PushCX()
{
    PushReg(1);
}

void PushDX()
{
    PushReg(2);
}

void PushBX()
{
    PushReg(3);
}

void PushSP()
{
    PushReg(4);
}

void PushBP()
{
    PushReg(5);
}

void PushSI()
{
    PushReg(6);
}

void PushDI()
{
    PushReg(7);
}

void PopAX()
{
    PopReg(0);
}

void PopCX()
{
    PopReg(1);
}

void PopDX()
{
    PopReg(2);
}

void PopBX()
{
    PopReg(3);
}

void PopSP()
{
    PopReg(4);
}

void PopBP()
{
    PopReg(5);
}

void PopSI()
{
    PopReg(6);
}

void PopDI()
{
    PopReg(7);
}
