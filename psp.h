#ifndef PSP_H
#define PSP_H

#include <cstdint>

void CreatePSP(uint16_t segment);
void SetCommandTail(uint16_t seg, const char *cmd);

#endif
