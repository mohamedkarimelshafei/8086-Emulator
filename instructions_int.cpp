#include "instructions_int.h"
#include "dos21.h"
#include "video.h"
#include "dos.h"
#include "bios.h"
#include "cpu.h"
#include <iostream>

using namespace std;
void IntImm()
{
    uint8_t number = Fetch8();
printf("INT %02X AH=%02X\n", number, cpu.ax >> 8);
    switch(number)
    {case 0x11:
    // BIOS equipment word
    cpu.ax = 0x0021;   // Standard PC with floppy
    break;
        case 0x21:
            cout << "\nCalling DOS AH=" << hex << ((cpu.ax >> 8) & 0xff) << "\n";
            DOS21();
            break;

        case 0x10:
            BIOSVideo();
            break;
case 0x13:
    BIOSInterrupt(0x13);
    break;

case 0x16:
    BIOSInterrupt(0x16);
    break;

case 0x12:
    BIOSInterrupt(0x12);
    break;
        default:
printf("INT %02X AH=%02X\n", number, cpu.ax >> 8);
            cpu.running = false;
            break;
    }
}
