#ifndef MOV8_H
#define MOV8_H

#include "cpu.h"

void MovRM8R8(uint8_t modrm);
void MovR8RM8(uint8_t modrm);
void Op88();
void Op8A();
#endif
