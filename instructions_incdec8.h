#ifndef INSTRUCTIONS_INCDEC8_H
#define INSTRUCTIONS_INCDEC8_H

#include <stdint.h>

void IncRM8(uint8_t modrm);
void DecRM8(uint8_t modrm);
void IncDec8();

#endif
