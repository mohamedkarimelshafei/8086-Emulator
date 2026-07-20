#ifndef INSTRUCTIONS_CMP_H
#define INSTRUCTIONS_CMP_H

#include "cpu.h"

void CmpAXImm();
void CmpRegReg(uint8_t modrm);
void CmpRegMem(uint8_t modrm);

#endif
