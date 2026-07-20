#include <iostream>
#include "opcode.h"

void ExecuteInstruction()
{
    uint8_t opcode = Fetch8();
printf("CS:IP=%04X:%04X FLAGS=%04X  ",
       cpu.cs, cpu.ip, cpu.flags);

uint32_t addr = ((uint32_t)cpu.cs << 4) + cpu.ip;

for(int i = 0; i < 8; i++)
    printf("%02X ", memory[addr+i]);

printf("\n");
    std::cout << "Executing opcode: 0x"
              << std::hex
              << (int)opcode
              << "\n";

    OpcodeTable[opcode]();
}
