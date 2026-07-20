#include <map>
#include <cstdio>
#include <cstring>
#include <string>
#include "cpu.h"
#include <iostream>
#include "interrupt.h"
#include <cstdio>
//static std::map<uint16_t,uint16_t> memoryBlocks;
FILE* files[32] = {};
static FILE *fcbFile = nullptr;
using namespace std;
uint16_t dtaSeg = 0;
uint16_t dtaOff = 0;
uint16_t currentPSP = 0x1000;
static uint32_t Ptr(uint16_t seg, uint16_t off)
{
    return ((uint32_t)seg << 4) + off;
}

void DOS21()
{
   uint8_t ah = cpu.ax >> 8;
cout << "\nINT 21 AH=" << hex << (int)ah
         << " AX=" << cpu.ax
         << " BX=" << cpu.bx
         << " CX=" << cpu.cx
         << " DX=" << cpu.dx
         << "\n";
cout << "\nINT 21 AH=" << hex << (int)ah << "\n";
switch(ah)
    {case 0x11:
{
    cout << "BIOS INT 11 equipment check" << endl;

    // Return a normal PC equipment word
    cpu.ax = 0x0000;

    break;
}
//static std::map<uint16_t,uint16_t> memoryBlocks;


case 0x4A:
{
    printf("DOS AH=4A resize %04X to %u paras\n",
           cpu.es, cpu.bx);

//    memoryBlocks[cpu.es] = cpu.bx;

    cpu.flags &= ~1;
    break;
}
case 0x49: // Free allocated memory
{
    uint16_t segment = cpu.es;

    printf("DOS AH=49 free memory segment=%04X\n", segment);

    // For now just mark success
    cpu.flags &= ~1; // clear carry flag

    break;
}
case 0x3E: // Close file
{
    uint16_t handle = cpu.bx;

    printf("DOS AH=3E close handle=%u\n", handle);

    // If you store handles in a table:
    if(handle < 32 && files[handle] != nullptr)
    {
        fclose(files[handle]);
        files[handle] = nullptr;
        cpu.flags &= ~1; // clear carry
    }
    else
    {
        cpu.flags |= 1;  // error
        cpu.ax = 6;      // invalid handle
    }

    break;
}
case 0x48:
{
    // BX = paragraphs requested
    uint16_t paragraphs = cpu.bx;

    static uint16_t nextFree = 0x2000;

    cpu.ax = nextFree;       // return segment
    nextFree += paragraphs;

    cpu.flags &= ~1;         // clear carry = success

    printf("DOS AH=48 allocate %u paras -> %04X\n",
           paragraphs, cpu.ax);

    break;
}
case 0x12:
{
    // KB of base memory
    cpu.ax = 640;
    break;
}
case 0x13:
{
    cout << "BIOS Disk INT 13 AH="
         << hex << ((cpu.ax >> 8)&0xff) << endl;

    cpu.flags &= ~1; // clear carry = success
    break;
}
case 0x36:
{
    cpu.ax = 512;   // sectors per cluster
    cpu.bx = 2800;  // free clusters
    cpu.cx = 512;   // bytes per sector
    cpu.dx = 2880;  // total clusters
    break;
}
case 0x4F:
{
    std::cout << "FindNext\n";

    // no more files
    cpu.flags |= 1;   // carry set = error

    break;
}
case 0x0E:
{
    // Return number of drives
    cpu.ax = (cpu.ax & 0xFF00) | 2; // A: and B:
    break;
}
case 0x3D:
{
    uint32_t addr = Address(cpu.ds, cpu.dx);

    char filename[128];
    strcpy(filename, (char*)&memory[addr]);

    std::cout << "Open: " << filename << "\n";

    cpu.ax = 5; // fake handle
    cpu.flags &= ~1;

    break;
}
case 0x3F:
{
    std::cout << "Read file handle="
              << cpu.bx << "\n";

    cpu.ax = cpu.cx; // pretend all bytes read
    cpu.flags &= ~1;

    break;
}
case 0x40:
{
    uint32_t addr = Address(cpu.ds, cpu.dx);

    std::cout << "Write " << cpu.cx << " bytes: ";

    for (uint16_t i = 0; i < cpu.cx; i++)
        std::cout << (char)memory[addr + i];

    std::cout << "\n";
    cpu.ax = cpu.cx;
    cpu.flags &= ~1;

    break;
}
case 0x41:
{
    std::cout << "Delete file\n";

    cpu.flags &= ~1;

    break;
}
case 0x4E:
{
    uint32_t pattern = Address(cpu.ds, cpu.dx);

    char name[128];
    strcpy(name, (char*)&memory[pattern]);

    std::cout << "FindFirst: " << name << "\n";

    // TODO: search your dosdisk directory

    cpu.flags &= ~1; // clear carry = success

    break;
}
case 0x27:
{
    uint16_t count = cpu.cx;

    while (count--)
    {
        // Read one 128-byte record using your AH=14 code.
        // If EOF occurs, stop and return AL = 1.
    }

    cpu.ax &= 0xFF00;   // AL = 0 (success)
    break;
}
case 0x30:
{
    // Report MS-DOS 5.00
    cpu.ax = 0x0005;   // AH = 5 (major), AL = 0 (minor)
    cpu.bx = 0;
    cpu.cx = 0;
    break;
}
case 0x51:
{
    cpu.bx = currentPSP;
    break;
}
case 0x50:
{
    currentPSP = cpu.bx;
    break;
}
case 0x07:
{
    cpu.ax = (cpu.ax & 0xFF00) | 0x0D;   // Return Enter
    break;
}
case 0x10:
{
    if (fcbFile)
    {
        fclose(fcbFile);
        fcbFile = nullptr;
    }

    cpu.ax &= 0xFF00;
    break;
}
case 0x14:
{
    if (!fcbFile)
    {
        cpu.ax = (cpu.ax & 0xFF00) | 1;
        break;
    }

    uint32_t dta = Ptr(dtaSeg, dtaOff);

    size_t n = fread(&memory[dta], 1, 128, fcbFile);

    if (n < 128)
        memset(&memory[dta + n], 0x1A, 128 - n);

    if (n == 0)
        cpu.ax = (cpu.ax & 0xFF00) | 1; // EOF
    else
        cpu.ax &= 0xFF00;               // Success

    break;
}
case 0x0F:
{
    uint32_t fcb = Ptr(cpu.ds, cpu.dx);
printf("FCB:\n");
for (int i = 0; i < 36; i++)
    printf("%02X ", memory[fcb + i]);
printf("\n");
cout << "Drive=" << dec << (int)memory[fcb] << endl;
    char name[9], ext[4];

    memcpy(name, &memory[fcb + 1], 8);
    memcpy(ext,  &memory[fcb + 9], 3);

    name[8] = 0;
    ext[3]  = 0;

    for (int i = 7; i >= 0 && name[i] == ' '; i--)
        name[i] = 0;

    for (int i = 2; i >= 0 && ext[i] == ' '; i--)
        ext[i] = 0;

    string filename = "dosdisk/";
    filename += name;

    if (ext[0])
    {
        filename += ".";
        filename += ext;
    }

    cout << "FCB OPEN: " << filename << endl;

    if (fcbFile)
    {
        fclose(fcbFile);
        fcbFile = nullptr;
    }

    fcbFile = fopen(filename.c_str(), "rb");

    if (!fcbFile)
    {
        cpu.ax = (cpu.ax & 0xFF00) | 0xFF;
        break;
    }

    // ---- Initialize FCB ----

    fseek(fcbFile, 0, SEEK_END);
    uint32_t size = ftell(fcbFile);
    fseek(fcbFile, 0, SEEK_SET);

    memory[fcb + 0x0C] = 0x80;
    memory[fcb + 0x0D] = 0x00;

    memory[fcb + 0x10] = size & 0xFF;
    memory[fcb + 0x11] = (size >> 8) & 0xFF;
    memory[fcb + 0x12] = (size >> 16) & 0xFF;
    memory[fcb + 0x13] = (size >> 24) & 0xFF;

    memset(&memory[fcb + 0x14], 0, 12);

    cpu.ax &= 0xFF00;
cout << "AL=" << hex << (cpu.ax & 0xFF) << endl;

cout << "FCB after open:";
for (int i = 0; i < 36; i++)
    printf(" %02X", memory[fcb + i]);
cout << endl;
    break;
}
        // Print string
        case 0x09:
        {
            uint32_t p = Ptr(cpu.ds, cpu.dx);
cout << "\nAH09 DS="
         << hex << cpu.ds
         << " DX="
         << cpu.dx
         << " ADDR="
         << p << "\n";
for(int i = 0; i < 32; i++)
{
    cout << hex << (int)memory[p+i] << " ";
}
cout << "\n";
            while(memory[p] != '$')
            {
                cout << (char)memory[p];
                p++;
            }

            break;
        }

        // Print character
        case 0x02:
        {
            char ch = cpu.dx & 0xFF;
            cout << ch;
            break;
        }
case 0x0C:
{
    uint8_t func = cpu.ax & 0xFF;

    switch(func)
    {
        case 0x01:
        case 0x06:
        case 0x07:
        case 0x08:
            // For now, pretend the user pressed Enter.
            cpu.ax = (cpu.ax & 0xFF00) | 0x0D;
            break;

        default:
            break;
    }

    break;
}
case 0x19:
{
    cpu.ax = (cpu.ax & 0xFF00) | 0;
    break;
}case 0x1A:
{
    dtaSeg = cpu.ds;
    dtaOff = cpu.dx;

    std::cout << "INT 21 AH=1A DTA="
              << std::hex << dtaSeg << ":" << dtaOff << "\n";
    break;
}
        // Exit program
        case 0x4C:
        {
            cpu.running = false;
            break;
        }
case 0x25:
{
    uint8_t intno = cpu.ax & 0xFF;   // AL
printf("SET VECTOR INT %02X = %04X:%04X\n",
           intno, cpu.ds, cpu.dx);

    ivt[intno].offset = cpu.dx;
    ivt[intno].segment = cpu.ds;
uint32_t addr = intno * 4;

memory[addr]     = cpu.dx & 0xFF;
memory[addr + 1] = cpu.dx >> 8;
memory[addr + 2] = cpu.ds & 0xFF;
memory[addr + 3] = cpu.ds >> 8;
    break;
}
case 0x26:
{
    // Create PSP
    // DX = segment where PSP should be created
    // Minimal implementation

    uint16_t seg = cpu.dx;

    // Set INT 20h instruction at PSP start
    uint32_t p = ((uint32_t)seg << 4);

    memory[p] = 0xCD;
    memory[p+1] = 0x20;

    break;
}
case 0x35:
{
    uint8_t intno = cpu.ax & 0xFF;
uint32_t addr = intno * 4;

cpu.bx = memory[addr] | (memory[addr+1] << 8);
cpu.es = memory[addr+2] | (memory[addr+3] << 8);
 printf("AH=35 INT %02X -> %04X:%04X\n",
           intno, cpu.es, cpu.bx);    
break;
}
default:
{
    cout << "\nDOS AH="
         << hex
         << (int)ah
         << " not implemented\n";

    cpu.running = false;
    break;
}
    }
}
