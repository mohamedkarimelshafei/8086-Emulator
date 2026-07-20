#include "vnc.h"
#include "cpu.h"
#include "loader.h"
#include "psp.h"
#include "opcode.h"
#include "executor.h"
#include <cstring>
#include <iostream>
#include <rfb/rfb.h>
#include "cga.h"
rfbScreenInfoPtr vnc;
uint8_t framebuffer[640 * 480 * 4];

void InitVNC()
{
    vnc = rfbGetScreen(
        NULL,
        NULL,
        640,
        480,
        8,
        3,
        4);

    vnc->frameBuffer = (char*)framebuffer;
    vnc->alwaysShared = TRUE;

    rfbInitServer(vnc);
//return true; 
}

void UpdateVNC()
{
    rfbProcessEvents(vnc,1000);
}
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "Usage: ./emu program.com\n";
        return 1;
    }

    ResetCPU();

    uint16_t pspSegment = 0x1000;

CreatePSP(pspSegment);
char cmdline[128] = " ";

for(int i = 2; i < argc; i++)
{
    if(i > 2)
        strcat(cmdline, " ");

    strcat(cmdline, argv[i]);
}

// SetCommandTail(0x1000, cmdline);
  if(!LoadEXE(argv[1]))
{
    if(!LoadCOM(argv[1]))
    {
        std::cout<<"Failed to load file\n";
        return 1;
    }
}
SetCommandTail(pspSegment, cmdline);
uint32_t p = Address(pspSegment, 0x80);

printf("PSP length=%u\n", memory[p]);
printf("Tail: ");

for (int i = 1; i <= memory[p]; i++)
    putchar(memory[p + i]);

putchar('\n');
    InitOpcodes();
InitVNC();
    std::cout << "8086 Emulator Started\n";

    while(cpu.running)
{
    ExecuteInstruction();
CGA_Render();
	UpdateVNC();
//    cpu.segmentOverride = false;
}

    std::cout << "CPU Halted\n";

    return 0;
}
