#include "instructions_jump.h"
#include "stack.h"
#include "flags.h"
void Ret()
{
    cpu.ip = Pop16();
}
void JeShort()
{
    int8_t offset = (int8_t)Fetch8();

    if(cpu.flags & 0x40)   // Zero Flag
    {
        cpu.ip += offset;
    }
}
void JmpShort()
{
    int8_t disp = (int8_t)Fetch8();
    cpu.ip += disp;
}

void JmpNear()
{
    int16_t disp = (int16_t)Fetch16();
    cpu.ip += disp;
}

void CallNear()
{
    int16_t disp = (int16_t)Fetch16();

    Push16(cpu.ip);

    cpu.ip += disp;
}

void RetNear()
{
    cpu.ip = Pop16();
}

void Je()
{
    int8_t disp = (int8_t)Fetch8();

    if(GetFlag(ZF))
        cpu.ip += disp;
}

void Jne()
{
    int8_t disp = (int8_t)Fetch8();

    if(!GetFlag(ZF))
        cpu.ip += disp;
}

void Jc()
{
    int8_t disp = (int8_t)Fetch8();

    if(GetFlag(CF))
        cpu.ip += disp;
}

void Jnc()
{
    int8_t disp = (int8_t)Fetch8();

    if(!GetFlag(CF))
        cpu.ip += disp;
}
