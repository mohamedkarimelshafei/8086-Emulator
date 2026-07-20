#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#define FLAG_CF 0x0001
#define FLAG_PF 0x0004
#define FLAG_AF 0x0010
#define FLAG_ZF 0x0040
#define FLAG_SF 0x0080
#define FLAG_TF 0x0100
#define FLAG_IF 0x0200
#define FLAG_DF 0x0400
#define FLAG_OF 0x0800
struct CPU
{
    // General registers
    uint16_t ax;
    uint16_t bx;
    uint16_t cx;
    uint16_t dx;

    // Index registers
    uint16_t si;
    uint16_t di;

    // Pointer registers
    uint16_t sp;
    uint16_t bp;

    // Segment registers
    uint16_t cs;
    uint16_t ds;
    uint16_t es;
    uint16_t ss;

    // Instruction Pointer
    uint16_t ip;

    // FLAGS
    uint16_t flags;

    // Running flag
    bool running;
bool segmentOverride;
uint16_t overrideSegment;
};

extern CPU cpu;

// 1 MB RAM
extern uint8_t memory[1024 * 1024];

// Register lookup table
extern uint16_t *reg16[8];

// Memory helpers
uint32_t Address(uint16_t seg, uint16_t off);

uint8_t Read8(uint16_t seg, uint16_t off);
uint16_t Read16(uint16_t seg, uint16_t off);

void Write8(uint16_t seg, uint16_t off, uint8_t value);
void Write16(uint16_t seg, uint16_t off, uint16_t value);

// Instruction fetch
uint8_t Fetch8();
uint16_t Fetch16();

// CPU
void ResetCPU();
void Execute();

#endif
