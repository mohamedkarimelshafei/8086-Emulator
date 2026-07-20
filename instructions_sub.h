#ifndef INSTRUCTIONS_SUB_H
#define INSTRUCTIONS_SUB_H

#include "cpu.h"

void SubAXImm();
void SubRegReg(uint8_t modrm);
void SubRegMem(uint8_t modrm);

#endif
