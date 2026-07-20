#ifndef FLAGS_H
#define FLAGS_H

#include "cpu.h"

#define CF 0x0001
#define PF 0x0004
#define AF 0x0010
#define ZF 0x0040
#define SF 0x0080
#define TF 0x0100
#define IF 0x0200
#define DF 0x0400
#define OF 0x0800

void SetFlag(uint16_t flag, bool value);
bool GetFlag(uint16_t flag);

void UpdateZeroSign16(uint16_t value);

#endif
