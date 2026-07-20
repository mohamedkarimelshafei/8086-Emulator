#ifndef MODRM_H
#define MODRM_H

#include "cpu.h"
//void SetRM16(uint8_t modrm, uint16_t value);
uint32_t GetEA(uint8_t mod, uint8_t rm);
//uint16_t GetRM16(uint8_t modrm);
//uint16_t GetRM16(uint8_t modrm, uint32_t *out_addr);
//void SetRM16(uint8_t modrm, uint16_t value, uint32_t addr);
uint16_t GetRM16(uint8_t modrm);
//uint16_t GetRM16(uint8_t modrm, uint32_t *out_addr);

void SetRM16(uint8_t modrm, uint16_t value);
//void SetRM16(uint8_t modrm, uint16_t value, uint32_t addr);
#endif
