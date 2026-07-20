#include <stdio.h>
#include "cpu.h"
#include <cstring>

CPU cpu;

uint8_t memory[1024 * 1024];

uint16_t *reg16[8] =
{
    &cpu.ax,
    &cpu.cx,
    &cpu.dx,
    &cpu.bx,
    &cpu.sp,
    &cpu.bp,
    &cpu.si,
    &cpu.di
};

uint32_t Address(uint16_t seg, uint16_t off)
{
    return (((uint32_t)seg << 4) + off) & 0xFFFFF;
}

uint8_t Read8(uint16_t seg, uint16_t off)
{
    return memory[Address(seg, off)];
}

uint16_t Read16(uint16_t seg, uint16_t off)
{
    uint16_t lo = Read8(seg, off);
    uint16_t hi = Read8(seg, off + 1);
    return lo | (hi << 8);
}

void Write8(uint16_t seg, uint16_t off, uint8_t value)
{
    memory[Address(seg, off)] = value;
}

void Write16(uint16_t seg, uint16_t off, uint16_t value)
{
if (seg == 0x1000 && off >= 0x0440 && off <= 0x0460)
{
    printf("WRITE %04X:%04X = %04X\n", seg, off, value);
}
if (seg == 0x1000 && off >= 0x0456 && off <= 0x045A)
        printf("WRITE %04X:%04X = %04X\n", seg, off, value);
    Write8(seg, off, value & 0xFF);
    Write8(seg, off + 1, value >> 8);
}

uint8_t Fetch8()
{
    return Read8(cpu.cs, cpu.ip++);
}

uint16_t Fetch16()
{
    uint16_t value = Read16(cpu.cs, cpu.ip);
    cpu.ip += 2;
    return value;
}
void ResetCPU()
{
    cpu.ax = 0;
    cpu.bx = 0;
    cpu.cx = 0;
    cpu.dx = 0;

    cpu.sp = 0xFFFE;
    cpu.bp = 0;

    cpu.si = 0;
    cpu.di = 0;

    cpu.cs = 0x1000;
    cpu.ds = 0x1000;
    cpu.es = 0x1000;
    cpu.ss = 0x1000;

//    cpu.ip = 0x0100;   // IMPORTANT
    cpu.flags = 0;

cpu.segmentOverride = false;
cpu.overrideSegment = cpu.ds;
    cpu.running = true;
}
void ReesetCPU()
{
    memset(memory, 0, sizeof(memory));
    memset(&cpu, 0, sizeof(cpu));

    cpu.cs = 0x1000;
    cpu.ds = 0x1000;
    cpu.es = 0x1000;
    cpu.ss = 0x1000;

    cpu.ip = 0x0100;
    cpu.sp = 0xFFFE;

    cpu.running = true;
}
