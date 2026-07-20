#include "psp.h"
#include "cpu.h"
#include <string.h>
void CreatePSP(uint16_t segment)
{
    uint32_t p = Address(segment, 0);

    memset(&memory[p], 0, 256);

    memory[p + 0] = 0xCD;
    memory[p + 1] = 0x20;
// End of allocated memory segment
    // Example: give COM program almost all conventional memory
    memory[p + 2] = 0xFF;
    memory[p + 3] = 0x9F;}
void SetCommandTail(uint16_t pspSeg, const char *cmd)
{
    uint32_t p = Address(pspSeg,0x80);

    int len = strlen(cmd);

    memory[p] = len;

    for(int i=0;i<len;i++)
        memory[p+1+i] = cmd[i];

    memory[p+1+len] = 0x0D;
}
