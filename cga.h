#ifndef CGA_H
#define CGA_H

#include <stdint.h>

void CGA_WritePort(uint16_t port,uint8_t value);
uint8_t CGA_ReadPort(uint16_t port);

void CGA_Render();

#endif
