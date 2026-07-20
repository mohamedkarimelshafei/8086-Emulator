#include "cpu.h"
#include "opcode.h"

extern OpcodeHandler OpcodeTable[256];

bool segmentOverride = false;
uint16_t overrideSegment;

void Prefix26()
{
    cpu.segmentOverride = true;
    cpu.overrideSegment = cpu.es;

    uint8_t opcode = Fetch8();

    OpcodeTable[opcode]();

    cpu.segmentOverride = false;
}
