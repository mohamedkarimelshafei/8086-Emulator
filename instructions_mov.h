#ifndef INSTRUCTIONS_MOV_H
#define INSTRUCTIONS_MOV_H

#include "cpu.h"

void MovRegReg(uint8_t modrm);
void MovRegMem(uint8_t modrm);
void MovMemReg(uint8_t modrm);

#endif
