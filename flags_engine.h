#ifndef FLAGS_ENGINE_H
#define FLAGS_ENGINE_H

#include "cpu.h"

void UpdateAdd16(uint16_t a,uint16_t b,uint32_t result);
void UpdateSub16(uint16_t a,uint16_t b,uint32_t result);

void UpdateAdd8(uint8_t a,uint8_t b,uint16_t result);
void UpdateSub8(uint8_t a,uint8_t b,uint16_t result);

bool Parity(uint8_t value);

#endif
