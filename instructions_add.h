#ifndef INSTRUCTIONS_ADD_H
#define INSTRUCTIONS_ADD_H

#include "cpu.h"

void AddRegReg(uint8_t modrm);
void AddRegMem(uint8_t modrm);
void AddAXImm();

#endif
