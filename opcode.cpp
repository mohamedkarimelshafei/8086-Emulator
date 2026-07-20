#include "cga.h"
#include "flags_engine.h"
#include "instructions_push_seg.h"
#include "instructions_loop.h"
#include "instructions_incdec8.h"
#include "instructions_group83.h"
#include "register8.h"
#include "instructions_mov8.h"
#include "instructions_mov_al_imm8.h"
#include "instructions_and_ax_imm16.h"
#include "instructions_prefix26.h"
#include "instructions_groupD3.h"
#include "jcc.h"
#include "instructions_flags2.h"
#include "instructions_groupF7.h"
#include "instructions_test.h"
#include "instructions_group80.h"
#include "instructions_mov_moffs8.h"
#include "modrm8.h"
#include "flags.h"
#include "modrm.h"
#include "pushpop.h"
#include "stack.h"
#include "instructions_mov_mem16.h"
#include "instructions_flags.h"
#include "instructions_aam.h"
#include "instructions_group81.h"
#include "instructions_mov_seg.h"
#include "instructions_incdec16.h"
#include "instructions_xor16.h"
#include "instructions_incdec8.h"
#include "instructions_or8.h"
#include "instructions_string.h"
#include "instructions_mov_mem8.h"
#include "instructions_int.h"
#include "instructions_mov_imm8.h"
#include "instructions_mov16.h"
#include "opcode.h"
#include <iostream>
#include "instructions_add.h"
#include "instructions_sub.h"
#include "instructions_jump.h"
#include "instructions_cmp8.h"
#include "instructions_xor8.h"
#include "instructions_sub16.h"
#include "instructions_cmp16.h"
#include "instructions_stack.h"
#define DF 0x0400
OpcodeHandler OpcodeTable[256];
extern void ExecuteJcc();
static void STOSW();
static void MOVSW();
static void TEST_AL_IMM8()
{
    uint8_t imm = Fetch8();
    uint8_t al = cpu.ax & 0xFF;

    uint8_t result = al & imm;

    // Clear CF and OF
    cpu.flags &= ~(CF | OF);

    // Zero Flag
    if (result == 0)
        cpu.flags |= ZF;
    else
        cpu.flags &= ~ZF;

    // Sign Flag
    if (result & 0x80)
        cpu.flags |= SF;
    else
        cpu.flags &= ~SF;

    // (PF can be added later if needed.)
}
void IN_IMM8_AL()
{
    uint8_t port = Read8(cpu.cs, cpu.ip++);

    uint8_t value = 0;

    if(port == 0x40)
    {
        // fake PIT timer value
        value = 0xFF;
        printf("[TIMER] IN 40h -> %02X\n", value);
    }
    else
    {
        printf("IN AL,%02X\n", port);
    }

    cpu.ax = (cpu.ax & 0xFF00) | value;
    //return value;
}
void IN_DX_AL()
{
    uint16_t port = cpu.dx;
    uint8_t value = 0;

    printf("IN DX(%04X),AL dummy\n", port);

    cpu.ax = (cpu.ax & 0xFF00) | value;
}
void OUT_DX_AL()
{
    uint16_t port = cpu.dx;
//    uint8_t value = cpu.al;

  uint8_t value = cpu.ax & 0xFF;
  if(port == 0x61)
    {
        printf("[SOUND] PC Speaker port=%04X value=%02X\n",
               port, value);
        return;
    }

    if(port == 0x388 || port == 0x389)
    {
        printf("[SOUND] AdLib port=%04X value=%02X\n",
               port, value);
        return;
    }

    if(port >= 0x220 && port <= 0x22F)
    {
        printf("[SOUND] SoundBlaster port=%04X value=%02X\n",
               port, value);
        return;
    }

    printf("OUT DX,AL port=%04X value=%02X\n",
           port,value);
}
void OUT_IMM_AL()
{
    uint8_t port = Read8(cpu.cs, cpu.ip++);
//    uint8_t value = cpu.al;

uint8_t value = cpu.ax & 0xFF;
    printf("[SOUND] OUT %02X,AL = %02X\n",
           port,value);
}
void OrALImm8()
{
   uint8_t imm = Fetch8();

uint8_t result = (cpu.ax & 0xFF) | imm;
cpu.ax = (cpu.ax & 0xFF00) | result;

SetFlag(CF, false);
SetFlag(OF, false);
SetFlag(AF, false);
SetFlag(ZF, result == 0);
SetFlag(SF, (result & 0x80) != 0);
SetFlag(PF, Parity(result));
}
static void SBB_R16_RM16()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    uint16_t src = GetRM16(modrm);

    uint16_t dst;

    switch(reg)
    {
        case 0: dst = cpu.ax; break;
        case 1: dst = cpu.cx; break;
        case 2: dst = cpu.dx; break;
        case 3: dst = cpu.bx; break;
        case 4: dst = cpu.sp; break;
        case 5: dst = cpu.bp; break;
        case 6: dst = cpu.si; break;
        case 7: dst = cpu.di; break;
    }

    uint32_t result = dst - src - ((cpu.flags & CF) ? 1 : 0);

    switch(reg)
    {
        case 0: cpu.ax = result; break;
        case 1: cpu.cx = result; break;
        case 2: cpu.dx = result; break;
        case 3: cpu.bx = result; break;
        case 4: cpu.sp = result; break;
        case 5: cpu.bp = result; break;
        case 6: cpu.si = result; break;
        case 7: cpu.di = result; break;
    }

    SetFlag(CF, result > 0xFFFF);
    SetFlag(ZF, (uint16_t)result == 0);
}
void ROL_RM8_1()
{
    uint8_t modrm = Fetch8();

    uint8_t mod = (modrm >> 6) & 3;
    uint8_t reg = (modrm >> 3) & 7;
    uint8_t rm  = modrm & 7;

    if(mod != 3)
    {
        printf("D0 memory not implemented\n");
        cpu.running = false;
        return;
    }

    uint8_t *r8[] =
{
    ((uint8_t*)&cpu.ax),       // AL
    ((uint8_t*)&cpu.cx),       // CL
    ((uint8_t*)&cpu.dx),       // DL
    ((uint8_t*)&cpu.bx),       // BL
    ((uint8_t*)&cpu.ax) + 1,   // AH
    ((uint8_t*)&cpu.cx) + 1,   // CH
    ((uint8_t*)&cpu.dx) + 1,   // DH
    ((uint8_t*)&cpu.bx) + 1    // BH
};

uint8_t v = *r8[rm];
    switch(reg)
    {
        case 0:
            v = (v << 1) | (v >> 7);
            break;

        case 1:
            v = (v >> 1) | (v << 7);
            break;

        case 4:
            v <<= 1;
            break;

        case 5:
            v >>= 1;
            break;

        case 7:
            v = ((int8_t)v) >> 1;
            break;

        default:
            printf("D0 /%d not implemented\n", reg);
            return;
    }

    *r8[rm] = v;
}
static void LEA()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    // Calculate effective address (offset only, no memory access)
    uint16_t offset = 0;

    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm  = modrm & 7;

    if(mod == 0 && rm == 6)
    {
        // direct [disp16]
        offset = Fetch16();
    }
    else
    {
        // TODO: expand other addressing modes
        printf("LEA unsupported ModRM=%02X\n", modrm);
        return;
    }

    switch(reg)
    {
        case 0: cpu.ax = offset; break;
        case 1: cpu.cx = offset; break;
        case 2: cpu.dx = offset; break;
        case 3: cpu.bx = offset; break;
        case 4: cpu.sp = offset; break;
        case 5: cpu.bp = offset; break;
        case 6: cpu.si = offset; break;
        case 7: cpu.di = offset; break;
    }
}
void LOOPE()
{
    int8_t disp = (int8_t)Fetch8();

    cpu.cx--;

    if (cpu.cx != 0 && (cpu.flags & FLAG_ZF))
        cpu.ip += disp;
}
static void STOSB()
{
    Write8(cpu.es, cpu.di, cpu.ax & 0xFF);

    if (cpu.flags & DF)
        cpu.di--;
    else
        cpu.di++;
}
static void SCASB();
void POP_ES()
{
    cpu.es = Pop16();
}
void REP()
{
    uint8_t opcode = Fetch8();

    switch(opcode)
    {case 0xAA:
    while (cpu.cx)
    {
        STOSB();
        cpu.cx--;
    }
    break;
case 0xA5:    // REP MOVSW
    while (cpu.cx)
    {
        MOVSW();
        cpu.cx--;
    }
    break;

case 0xAE:   // REPE SCASB
while (cpu.cx)
{
    SCASB();
    cpu.cx--;

    if (!(cpu.flags & ZF))
        break;
}
break;
        case 0xA4: // REP MOVSB
            while(cpu.cx)
            {
                Write8(cpu.es, cpu.di, Read8(cpu.ds, cpu.si));

                if (cpu.flags & DF)
                {
                    cpu.si--;
                    cpu.di--;
                }
                else
                {
                    cpu.si++;
                    cpu.di++;
                }

                cpu.cx--;
            }
            break;

        case 0xAB: // REP STOSW
            while(cpu.cx)
            {
                STOSW();
                cpu.cx--;
            }
            break;

        default:
            std::cout << "REP opcode "
                      << std::hex
                      << (int)opcode
                      << " not implemented\n";
            cpu.running = false;
            break;
    }
}
static void MOVSB()
{
    uint8_t value = Read8(cpu.ds, cpu.si);

    Write8(cpu.es, cpu.di, value);

    if (cpu.flags & 0x0400)
    {
        cpu.si--;
        cpu.di--;
    }
    else
    {
        cpu.si++;
        cpu.di++;
    }
}
static void SCASB()
{
    uint8_t value = Read8(cpu.es, cpu.di);
    uint8_t al = cpu.ax & 0xFF;

    uint8_t result = al - value;

    // Zero Flag
    if (result == 0)
        cpu.flags |= ZF;
    else
        cpu.flags &= ~ZF;

    // Sign Flag
    if (result & 0x80)
        cpu.flags |= SF;
    else
        cpu.flags &= ~SF;

    // Carry Flag
    if (al < value)
        cpu.flags |= CF;
    else
        cpu.flags &= ~CF;

    if (cpu.flags & DF)
        cpu.di--;
    else
        cpu.di++;
}
void XCHG_AX_DI()
{
    uint16_t temp = cpu.ax;
    cpu.ax = cpu.di;
    cpu.di = temp;
}
void RETF()
{
    cpu.ip = Pop16();
    cpu.cs = Pop16();
}
void PUSH_CS()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.cs);
}
static void STOSW()
{
    Write16(cpu.es, cpu.di, cpu.ax);

    if (cpu.flags & DF)
        cpu.di -= 2;
    else
        cpu.di += 2;
}
static void POP_AX() { cpu.ax = Pop16(); }
static void POP_CX() { cpu.cx = Pop16(); }
static void POP_DX() { cpu.dx = Pop16(); }
static void POP_BX() { cpu.bx = Pop16(); }
static void POP_SP() { cpu.sp = Pop16(); }   // 8086 semantics
static void POP_BP() { cpu.bp = Pop16(); }
static void POP_SI() { cpu.si = Pop16(); }
static void POP_DI() { cpu.di = Pop16(); }
void Prefix2E()
{
    cpu.segmentOverride = true;
    cpu.overrideSegment = cpu.cs;

    uint8_t opcode = Fetch8();

    OpcodeTable[opcode]();

    cpu.segmentOverride = false;
}
void AND_AL_IMM8()
{
    uint8_t imm = Fetch8();

    uint8_t al = cpu.ax & 0xff;

    al &= imm;

    cpu.ax = (cpu.ax & 0xff00) | al;

    cpu.flags &= ~(0x40 | 0x80);

    if(al == 0)
        cpu.flags |= 0x40;

    if(al & 0x80)
        cpu.flags |= 0x80;
}
static void PUSH_AX()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.ax);
}
static void JCXZ()
{
    int8_t disp = (int8_t)Fetch8();

    if (cpu.cx == 0)
        cpu.ip += disp;
}
static void PUSH_CX()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.cx);
}

static void PUSH_DX()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.dx);
}

static void PUSH_BX()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.bx);
}
static void MOVSW()
{
    uint16_t value = Read16(cpu.ds, cpu.si);

    Write16(cpu.es, cpu.di, value);

    if (cpu.flags & 0x0400)   // DF set
    {
        cpu.si -= 2;
        cpu.di -= 2;
    }
    else
    {
        cpu.si += 2;
        cpu.di += 2;
    }
}
static void PUSH_SP()
{
    uint16_t old_sp = cpu.sp;
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, old_sp);
}

static void PUSH_BP()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.bp);
}

static void PUSH_SI()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.si);
}

static void PUSH_DI()
{
    cpu.sp -= 2;
    Write16(cpu.ss, cpu.sp, cpu.di);
}
void ADD_AL_IMM8()
{
    uint8_t v = Fetch8();
    uint8_t al = cpu.ax & 0xff;

    al += v;

    cpu.ax = (cpu.ax & 0xff00) | al;
}
void MovReg8Imm8()
{
    uint8_t opcode = Read8(cpu.cs, cpu.ip - 1); // opcode already fetched
    uint8_t reg = opcode - 0xB0;
    uint8_t value = Fetch8();

    SetReg8(reg, value);
}
static void GROUP_D1()
{
    uint8_t modrm = Fetch8();

    if (modrm == 0xE9)   // SHR CX,1
    {
        if (cpu.cx & 1)
            cpu.flags |= CF;
        else
            cpu.flags &= ~CF;

        cpu.cx >>= 1;

        if (cpu.cx == 0)
            cpu.flags |= ZF;
        else
            cpu.flags &= ~ZF;

        return;
    }

    printf("Unhandled D1 ModRM=%02X\n", modrm);
    cpu.running = false;
}
void FEGroup()
{
    uint8_t modrm = Fetch8();

    uint8_t reg = (modrm >> 3) & 7;

    switch(reg)
    {
        case 0:
            IncRM8(modrm);
            break;

        case 1:
            DecRM8(modrm);
            break;

        default:
            printf("FE group %d not implemented\n", reg);
            cpu.running=false;
    }
}
static void GROUP_FF()
{
    uint8_t modrm = Fetch8();
uint8_t mod = modrm >> 6;
uint8_t reg = (modrm >> 3) & 7;
uint8_t rm  = modrm & 7;
if (mod == 0 && rm == 6)
{
    uint16_t disp = Fetch16();
    uint32_t addr = Address(cpu.ds, disp);

    uint16_t newIP = Read16(cpu.ds, disp);
    uint16_t newCS = Read16(cpu.ds, disp + 2);

    switch (reg)
    {
        case 3: // CALL FAR m16:16
            printf("CALL FAR -> %04X:%04X\n", newCS, newIP);

            Push16(cpu.cs);
            Push16(cpu.ip);

            cpu.cs = newCS;
            cpu.ip = newIP;
            return;

        case 5: // JMP FAR m16:16
            printf("JMP FAR -> %04X:%04X\n", newCS, newIP);

            cpu.cs = newCS;
            cpu.ip = newIP;
            return;
    }
}

    printf("Unhandled FF ModRM=%02X\n", modrm);
    cpu.running = false;
}
void MOV_RM8_IMM8()
{
    uint8_t modrm = Fetch8();

    if (((modrm >> 3) & 7) != 0)
    {
        std::cout << "Unsupported C6 /"
                  << ((modrm >> 3) & 7)
                  << std::endl;
if (((modrm >> 3) & 7) != 0)
{
    printf("Invalid C6 opcode\n");
    exit(1);
}        
        return;
    }

    uint8_t value = Fetch8();

    SetRM8(modrm, value);
}
static void LOOP()
{
    int8_t disp = (int8_t)Fetch8();

    cpu.cx--;

    if (cpu.cx != 0)
        cpu.ip += disp;
}
static void INC_AX() { IncReg16(0); }
static void INC_CX() { IncReg16(1); }
static void INC_DX() { IncReg16(2); }
static void INC_BX() { IncReg16(3); }
static void INC_SP() { IncReg16(4); }
static void INC_BP() { IncReg16(5); }
static void INC_SI() { IncReg16(6); }
static void INC_DI() { IncReg16(7); }
void MovCLImm8()
{
    uint8_t value = Fetch8();
    cpu.cx = (cpu.cx & 0xFF00) | value;
}
static void POP_DS()
{
    cpu.ds = Pop16();
}
static void PUSH_DS()
{
    Push16(cpu.ds);
}
void MOV_MOFFS16_AX()
{
    MovMoffs16AX();
}
static void CMP_AX_IMM16()
{
    CmpAXImm16();
}
static void SUB_R16_RM16()
{
    SubR16Rm16();
}
static void ADD_R16_RM16()
{
    uint8_t modrm = Fetch8();
    AddRegMem(modrm);
}
static void MOV_RM16_IMM16()
{
    uint8_t modrm = Fetch8();

    uint16_t value = Fetch16();

    uint8_t mod = (modrm >> 6) & 3;
    uint8_t rm  = modrm & 7;

    if (mod == 3)
    {
        // register destination
        *reg16[rm] = value;
    }
    else
    {
        // memory destination
        SetRM16(modrm, value);
    }
}
static void MOV_AX_MEM16()
{
    MovAXMem16();
}
static void XOR_R8_RM8()
{
    XorR8Rm8();
}
static void MOV_R16_RM16()
{
    MovR16Rm16();
}
static void MOV_RM16_R16()
{
    MovRm16R16();
}
static void CLI_INST()
{
    CLI();
}

static void STI_INST()
{
    STI();
}
static void MOV_SEG_RM16()
{
    MovSegRm16();
}
static void AAM_INST()
{
    AAM();
}
static void GROUP81()
{
    Group81();
}
static void GROUP80()
{
    Group80();
}
static void MOV_RM16_SEG()
{
    MovRm16Seg();
}
static void JMP_NEAR()
{
    JmpNear();
}
static void RET()
{
    Ret();
}
static void DEC_AX(){ DecReg16(0); }
static void DEC_CX(){ DecReg16(1); }
static void DEC_DX(){ DecReg16(2); }
static void DEC_BX(){ DecReg16(3); }
static void DEC_SP(){ DecReg16(4); }
static void DEC_BP(){ DecReg16(5); }
static void DEC_SI(){ DecReg16(6); }
static void DEC_DI(){ DecReg16(7); }
static void JE_SHORT()
{
    JeShort();
}
static void CALL_NEAR()
{
    CallNear();
}
static void XOR_R16_RM16()
{
    Xor16();
}
static void INCDEC8()
{
    IncDec8();
}
static void OR_R8_RM8()
{
    Or8();
}
static void LODSB()
{
    Lodsb();
}
static void MOV_BP_IMM()
{
    cpu.bp = Fetch16();
}

static void MOV_SI_IMM()
{
    cpu.si = Fetch16();
}

static void MOV_DI_IMM()
{
    cpu.di = Fetch16();
}
static void MOV_MEM8_AL()
{
    MovMem8AL();
}
static void INT_IMM()
{
    IntImm();
}
static void MOV_AH_IMM()
{
    MovR8Imm(4);
}
static void JNE_SHORT()
{
    Jne();
}
static void MOV_SP_IMM()
{
    MovSPImm();
}
static void CMP_AL_IMM()
{
    CmpALImm();
}
static void InvalidOpcode()
{
    std::cout << "Unknown opcode\n";
    cpu.running = false;
}

static void NOP()
{
}

static void HLT()
{
    cpu.running = false;
}

static void MOV_AX_IMM()
{
    cpu.ax = Fetch16();
}

static void MOV_CX_IMM()
{
    cpu.cx = Fetch16();
}

static void MOV_DX_IMM()
{
    cpu.dx = Fetch16();
}

static void MOV_BX_IMM()
{
    cpu.bx = Fetch16();
}

static void ADD_AX_IMM()
{
    AddAXImm();
}

static void SUB_AX_IMM()
{
    SubAXImm();
}

static void JMP_SHORT()
{
    JmpShort();
}

void InitOpcodes()
{
    for(int i = 0; i < 256; i++)
 OpcodeTable[i] = InvalidOpcode;
OpcodeTable[0xD4] = AAM_INST;
OpcodeTable[0xE9] = JMP_NEAR;
    OpcodeTable[0x90] = NOP;
    OpcodeTable[0xF4] = HLT;
OpcodeTable[0xAD] = LODSW;
OpcodeTable[0xFC] = CLD;
OpcodeTable[0x58] = POP_AX;
OpcodeTable[0x59] = POP_CX;
OpcodeTable[0x5A] = POP_DX;
OpcodeTable[0x5B] = POP_BX;
OpcodeTable[0x5C] = POP_SP;
OpcodeTable[0x5D] = POP_BP;
OpcodeTable[0x5E] = POP_SI;
OpcodeTable[0x5F] = POP_DI;
    OpcodeTable[0xA2] = MOV_MEM8_AL;
    OpcodeTable[0xB8] = MOV_AX_IMM;
    OpcodeTable[0xB9] = MOV_CX_IMM;
    OpcodeTable[0xBA] = MOV_DX_IMM;
    OpcodeTable[0xBB] = MOV_BX_IMM;
    OpcodeTable[0xBD] = MOV_BP_IMM;
OpcodeTable[0xE2] = LOOP;
OpcodeTable[0xBE] = MOV_SI_IMM;
OpcodeTable[0xBF] = MOV_DI_IMM;
    OpcodeTable[0x05] = ADD_AX_IMM;
    OpcodeTable[0x2D] = SUB_AX_IMM;
    OpcodeTable[0x75] = JNE_SHORT;
    OpcodeTable[0xEB] = JMP_SHORT;
    OpcodeTable[0x3C] = CMP_AL_IMM;
    OpcodeTable[0xBC] = MOV_SP_IMM;
    OpcodeTable[0xB4] = MOV_AH_IMM;
    OpcodeTable[0xCD] = INT_IMM;
OpcodeTable[0xD1] = GROUP_D1;
OpcodeTable[0xEE] = OUT_DX_AL;
OpcodeTable[0xA8] = TEST_AL_IMM8;
OpcodeTable[0xAC] = LODSB;
OpcodeTable[0xF3] = REP;
OpcodeTable[0xC6] = MOV_RM8_IMM8;
OpcodeTable[0x97] = XCHG_AX_DI;
OpcodeTable[0x0A] = OR_R8_RM8;
OpcodeTable[0xFE] = INCDEC8;
OpcodeTable[0x06] = PUSH_ES;
OpcodeTable[0x33] = XOR_R16_RM16;
OpcodeTable[0xE8] = CALL_NEAR;
OpcodeTable[0x03] = ADD_R16_RM16;
OpcodeTable[0x74] = JE_SHORT;
OpcodeTable[0x48] = DEC_AX;
OpcodeTable[0x49] = DEC_CX;
OpcodeTable[0x2E] = Prefix2E;
OpcodeTable[0x4A] = DEC_DX;
OpcodeTable[0x4B] = DEC_BX;
OpcodeTable[0x4C] = DEC_SP;
OpcodeTable[0x07] = POP_ES;
OpcodeTable[0x4D] = DEC_BP;
OpcodeTable[0x4E] = DEC_SI;
OpcodeTable[0x4F] = DEC_DI;
OpcodeTable[0xC3] = RET;
OpcodeTable[0x8C] = MOV_RM16_SEG;
OpcodeTable[0x81] = GROUP81;
OpcodeTable[0x8E] = MOV_SEG_RM16;
OpcodeTable[0xFA] = CLI_INST;
OpcodeTable[0xFB] = STI_INST;
//opcodeTable[0x09] = OrRm16R16;
OpcodeTable[0x89] = MOV_RM16_R16;
OpcodeTable[0xE3] = JCXZ;
OpcodeTable[0xE1] = LOOPE;
OpcodeTable[0x8B] = MOV_R16_RM16;
OpcodeTable[0x32] = XOR_R8_RM8;
OpcodeTable[0xE4] = IN_IMM8_AL;
OpcodeTable[0xEC] = IN_DX_AL;
OpcodeTable[0xA1] = MOV_AX_MEM16;
OpcodeTable[0xCB] = RETF;
OpcodeTable[0x2B] = SUB_R16_RM16;
OpcodeTable[0x8D] = LEA;
OpcodeTable[0x3D] = CMP_AX_IMM16;
OpcodeTable[0xA3] = MOV_MOFFS16_AX;
OpcodeTable[0xFE] = FEGroup;
OpcodeTable[0xD0] = ROL_RM8_1;
OpcodeTable[0x1E] = PUSH_DS;
OpcodeTable[0x1F] = POP_DS;
OpcodeTable[0x56] = PushSI;
OpcodeTable[0x0E] = PUSH_CS;
OpcodeTable[0xB1] = MovCLImm8;
OpcodeTable[0x30] = XOR_RM8_R8;
OpcodeTable[0xC7] = MOV_RM16_IMM16;
OpcodeTable[0x40] = INC_AX;
OpcodeTable[0x41] = INC_CX;
OpcodeTable[0x42] = INC_DX;
OpcodeTable[0x43] = INC_BX;
OpcodeTable[0x44] = INC_SP;
OpcodeTable[0x45] = INC_BP;
OpcodeTable[0x46] = INC_SI;
OpcodeTable[0x47] = INC_DI;
OpcodeTable[0x24] = AND_AL_IMM8;
OpcodeTable[0xFF] = GROUP_FF;
OpcodeTable[0xA0] = MOV_AL_MEM8;
OpcodeTable[0xA2] = MOV_MEM8_AL;
OpcodeTable[0xE0] = LoopNZ;
OpcodeTable[0x04] = ADD_AL_IMM8;
OpcodeTable[0x80] = GROUP80;
OpcodeTable[0xA9] = TEST_AX_IMM16;
OpcodeTable[0xF7] = GROUPF7;
OpcodeTable[0xF8] = CLC;
OpcodeTable[0x72] = ExecuteJcc;
OpcodeTable[0x73] = ExecuteJcc;
OpcodeTable[0x74] = ExecuteJcc;
OpcodeTable[0x75] = ExecuteJcc;
OpcodeTable[0xA5] = MOVSW;
OpcodeTable[0x76] = ExecuteJcc;
OpcodeTable[0x77] = ExecuteJcc;
OpcodeTable[0x78] = ExecuteJcc;
OpcodeTable[0x79] = ExecuteJcc;
OpcodeTable[0x7A] = ExecuteJcc;
OpcodeTable[0x7B] = ExecuteJcc;
OpcodeTable[0x7C] = ExecuteJcc;
OpcodeTable[0x7D] = ExecuteJcc;
OpcodeTable[0x7E] = ExecuteJcc;
OpcodeTable[0x7F] = ExecuteJcc;
OpcodeTable[0xD3] = GroupD3;
OpcodeTable[0xF3] = REP;
OpcodeTable[0x26] = Prefix26;
OpcodeTable[0x25] = AndAXImm16;
OpcodeTable[0xB0] = MovALImm8;
OpcodeTable[0x88] = Op88;
OpcodeTable[0x8A] = Op8A;
OpcodeTable[0xB0] = MovReg8Imm8;
OpcodeTable[0xB1] = MovReg8Imm8;
OpcodeTable[0xB2] = MovReg8Imm8;
OpcodeTable[0xB3] = MovReg8Imm8;
OpcodeTable[0x0C] = OrALImm8;
OpcodeTable[0xB4] = MovReg8Imm8;
OpcodeTable[0xB5] = MovReg8Imm8;
//OpcodeTable[0xEC]=IN_DX_AL;
OpcodeTable[0xB6] = MovReg8Imm8;
OpcodeTable[0xB7] = MovReg8Imm8;
OpcodeTable[0x83] = Group83;
OpcodeTable[0xF8] = CLC;
OpcodeTable[0xF9] = STC;
OpcodeTable[0xFA] = CLI;
OpcodeTable[0xFB] = STI;
OpcodeTable[0xFC] = CLD;
OpcodeTable[0xFD] = STD;
OpcodeTable[0x9C] = PUSHF;
OpcodeTable[0x9D] = POPF;
OpcodeTable[0x50] = PUSH_AX;
OpcodeTable[0x51] = PUSH_CX;
OpcodeTable[0x52] = PUSH_DX;
OpcodeTable[0x53] = PUSH_BX;
OpcodeTable[0x54] = PUSH_SP;
OpcodeTable[0x55] = PUSH_BP;
OpcodeTable[0x56] = PUSH_SI;
OpcodeTable[0x57] = PUSH_DI;
OpcodeTable[0x1B] = SBB_R16_RM16;
OpcodeTable[0xE6] = OUT_IMM_AL;}
