#include "cpu.h"
#include <fstream>
#include <iostream>
#include "loader.h"
#pragma pack(push,1)
struct EXEHeader
{
    uint16_t magic;
    uint16_t bytesLastPage;
    uint16_t pages;
    uint16_t relocations;
    uint16_t headerSize;
    uint16_t minAlloc;
    uint16_t maxAlloc;
    uint16_t ss;
    uint16_t sp;
    uint16_t checksum;
    uint16_t ip;
    uint16_t cs;
    uint16_t relocTable;
};
#pragma pack(pop)


bool LoadEXE(const char *filename)
{std::cout << "EXE LOADER ACTIVE\n";
std::cout << "Trying EXE: " << filename << "\n";

    std::ifstream file(filename, std::ios::binary);

    if(!file)
    {
        std::cout << "Open failed\n";
        return false;
    }

    std::cout << "Opened OK\n";
    
    if(!file)
        return false;

    EXEHeader h;

    file.read((char*)&h, sizeof(h));

    if(h.magic != 0x5A4D)
{
    std::cout << "Not an MZ EXE - loading COM\n";

    file.clear();
    file.seekg(0);

    uint16_t loadSeg = 0x1000;

    uint32_t addr = Address(loadSeg, 0x100);

    file.read((char*)&memory[addr], 0x10000 - 0x100);

    cpu.cs = loadSeg;
    cpu.ip = 0x0100;

    cpu.ss = loadSeg;
    cpu.sp = 0xFFFE;

    return true;
}
   std::cout << "MZ EXE detected\n";

std::cout << "headerSize=" << std::hex << h.headerSize << "\n";
std::cout << "CS=" << h.cs << " IP=" << h.ip << "\n";
std::cout << "SS=" << h.ss << " SP=" << h.sp << "\n";

uint16_t loadSeg = 0x1010;
uint32_t loadAddr = Address(loadSeg, 0);

file.seekg(h.headerSize * 16);

uint32_t imageSize = h.pages * 512 - h.headerSize * 16;

if(h.bytesLastPage)
    imageSize -= (512 - h.bytesLastPage);

file.read((char*)&memory[loadAddr], imageSize);
// Apply relocation table
file.seekg(h.relocTable);

for(int i = 0; i < h.relocations; i++)
{
    uint16_t off, seg;

    file.read((char*)&off, 2);
    file.read((char*)&seg, 2);

    uint32_t addr = Address(loadSeg + seg, off);

    uint16_t value = memory[addr] | (memory[addr+1] << 8);
    value += loadSeg;

    memory[addr] = value & 0xff;
    memory[addr+1] = value >> 8;
}
// Initial registers
cpu.cs = loadSeg + h.cs;
cpu.ip = h.ip;

cpu.ss = loadSeg + h.ss;
cpu.sp = h.sp;

    // DOS normally gives PSP segment in DS/ES
    cpu.ds = 0x1000;
    cpu.es = 0x1000;


    std::cout << "Entry CS:IP="
              << std::hex
              << cpu.cs << ":"
              << cpu.ip
              << "\n";

    return true;
}
