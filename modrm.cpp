#include <stdio.h>
#include "modrm.h"
uint32_t last_ea;
bool last_ea_valid = false;
//printf("EA mod=%d rm=%d IP=%04X\n", mod, rm, cpu.ip);
uint32_t GetEA(uint8_t mod, uint8_t rm)
{ printf("EA mod=%d rm=%d IP=%04X\n", mod, rm, cpu.ip);
    uint16_t base = 0;
    uint16_t seg = cpu.ds;

    switch(rm)
    {
        case 0: base = cpu.bx + cpu.si; break;
        case 1: base = cpu.bx + cpu.di; break;

        case 2:
            base = cpu.bp + cpu.si;
            seg = cpu.ss;
            break;

        case 3:
            base = cpu.bp + cpu.di;
            seg = cpu.ss;
            break;

        case 4:
            base = cpu.si;
            break;

        case 5:
            base = cpu.di;
            break;

        case 6:
            if(mod == 0)
            {
                uint16_t disp = Fetch16();
                return Address(cpu.ds, disp);
            }
            base = cpu.bp;
            seg = cpu.ss;
            break;

        case 7:
            base = cpu.bx;
            break;
    }

    if(mod == 1)
        base += (int8_t)Fetch8();

    if(mod == 2)
        base += Fetch16();
last_ea = Address(seg, base);
last_ea_valid = true;
return last_ea;
//    return Address(seg, base);
}
uint16_t GetRM16(uint8_t modrm, uint32_t *out_addr)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm = modrm & 7;

    if(mod == 3)
    {
        *out_addr = 0xffffffff;
        switch(rm)
        {
            case 0:return cpu.ax;
            case 1:return cpu.cx;
            case 2:return cpu.dx;
            case 3:return cpu.bx;
            case 4:return cpu.sp;
            case 5:return cpu.bp;
            case 6:return cpu.si;
            case 7:return cpu.di;
        }
    }

    uint32_t addr = GetEA(mod, rm);
    *out_addr = addr;

    return memory[addr] | (memory[addr+1]<<8);
}

void SetRM16(uint8_t modrm, uint16_t value, uint32_t addr)
{
    if(addr == 0xffffffff)
    {
        uint8_t rm = modrm & 7;
        switch(rm)
        {
            case 0: cpu.ax=value; break;
            case 1: cpu.cx=value; break;
            case 2: cpu.dx=value; break;
            case 3: cpu.bx=value; break;
            case 4: cpu.sp=value; break;
            case 5: cpu.bp=value; break;
            case 6: cpu.si=value; break;
            case 7: cpu.di=value; break;
        }
        return;
    }

    memory[addr] = value & 0xff;
    memory[addr+1] = value >> 8;
}

/*
int32_t addr;

if(last_ea_valid)
{
    addr = last_ea;
    last_ea_valid = false;
}
else
{
    addr = GetEA(mod, rm);
}
    uint32_t addr = GetEA(mod, rm);
    memory[addr] = value & 0xff;
    memory[addr+1] = value >> 8;
}
void SeetRM16(uint8_t modrm, uint16_t value)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm  = modrm & 7;

    if(mod == 3)
    {
        switch(rm)
        {
            case 0: cpu.ax=value; break;
            case 1: cpu.cx=value; break;
            case 2: cpu.dx=value; break;
            case 3: cpu.bx=value; break;
            case 4: cpu.sp=value; break;
            case 5: cpu.bp=value; break;
            case 6: cpu.si=value; break;
            case 7: cpu.di=value; break;
        }
        return;
    }

    uint32_t addr = GetEA(mod, rm);

    memory[addr] = value & 0xff;
    memory[addr+1] = value >> 8;
}
 1: cpu.cx=value; break;
            case 2: cpu.dx=value; break;
            case 3: cpu.bx=value; break;
            case 4: cpu.sp=value; break;
            case 5: cpu.bp=value; break;
            case 6: cpu.si=value; break;
            case 7: cpu.di=value; break;
        }
        return;
    }
/*uint32_t addr;

if(last_ea_valid)
{
    addr = last_ea;
    last_ea_valid = false;
}
else
{
    addr = GetEA(mod, rm);
}
//    uint32_t addr = GetEA(mod, rm);
    memory[addr] = value & 0xff;
    memory[addr+1] = value >> 8;
}
void SeetRM16(uint8_t modrm, uint16_t value)
{
    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm  = modrm & 7;

    if(mod == 3)
    {
        switch(rm)
        {
            case 0: cpu.ax=value; break;
            case 1: cpu.cx=value; break;
            case 2: cpu.dx=value; break;
            case 3: cpu.bx=value; break;
            case 4: cpu.sp=value; break;
            case 5: cpu.bp=value; break;
            case 6: cpu.si=value; break;
            case 7: cpu.di=value; break;
        }
        return;
    }

    uint32_t addr = GetEA(mod, rm);

    memory[addr] = value & 0xff;
    memory[addr+1] = value >> 8;
}
*/
uint16_t GetRM16(uint8_t modrm)
{
    uint32_t dummy;
    return GetRM16(modrm, &dummy);
}

void SetRM16(uint8_t modrm, uint16_t value)
{
    uint32_t addr;
    GetRM16(modrm, &addr);
    SetRM16(modrm, value, addr);
}
