#ifndef OPCODE_H
#define OPCODE_H

#include "cpu.h"

typedef void (*OpcodeHandler)();

extern OpcodeHandler OpcodeTable[256];

void InitOpcodes();

#endif
