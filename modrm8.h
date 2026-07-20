#ifndef MODRM8_H
#define MODRM8_H

#include "cpu.h"

uint8_t ReadRM8(uint8_t mod, uint8_t rm);
void WriteRM8(uint8_t mod, uint8_t rm, uint8_t value);
uint8_t GetRM8(uint8_t modrm);
void SetRM8(uint8_t modrm, uint8_t value);
#endif
