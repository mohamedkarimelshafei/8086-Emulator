#include "bios.h"
#include "cpu.h"
#include <iostream>
#include <fstream>

using namespace std;
static uint8_t videoMode = 3;   // default 80x25 text
static uint16_t cursorX = 0;
static uint16_t cursorY = 0;
void BIOSInterrupt(unsigned char number)
{
    switch(number)
    {case 0x01: // Get status
{
    cout << "INT13 Get Status\n";

    cpu.ax &= 0x00FF;   // AH=0 success
    cpu.flags &= ~1;    // clear CF

    break;
}
case 0x12:
//        printf("INT10 AH=12 AL=%02X BL=%02X\n", cpu.al, cpu.bl);
printf("INT10 AH=12 AL=%02X BL=%02X\n",
       cpu.ax & 0xFF,
       cpu.bx & 0xFF);
        // Pretend no EGA/VGA is present.
        cpu.bx = 0;
        cpu.flags &= ~1; // CF=0
        break;
case 0x02:
{
    cursorX = cpu.dx & 0xFF;
    cursorY = cpu.dx >> 8;
    break;
}
case 0x03:
{
    cpu.dx = (cursorY << 8) | cursorX;
    break;
}
case 0x09:
{
    char ch = cpu.ax & 0xFF;
    uint16_t count = cpu.cx;

    while(count--)
        putchar(ch);

    fflush(stdout);
    break;
}
case 0x06:
{
    if((cpu.ax & 0xFF) == 0)
        system("clear");    // Linux terminal

    break;
}
case 0x04: // Verify sectors
{
    uint8_t count = cpu.ax & 0xff;

    cout << "INT13 Verify "
         << (int)count << " sectors\n";

    cpu.ax &= 0x00FF;   // AH=0
    cpu.flags &= ~1;    // success

    break;
}
case 0x0C:      // Write pixel
{
    uint16_t x = cpu.cx;
    uint16_t y = cpu.dx;
    uint8_t color = cpu.ax & 0xFF;

    // TODO: draw pixel in your framebuffer
    break;
}

case 0x0D:      // Read pixel
{
    // TODO
    cpu.ax = (cpu.ax & 0xFF00) | 0;
    break;
}
case 0x11:
    cpu.ax = 0x0001;
    break;
        case 0x10:
{
    uint8_t ah = cpu.ax >> 8;
printf("INT10 AH=%02X\n", (cpu.ax >> 8) & 0xFF);
    switch(ah)
    {
        case 0x00:      // Set video mode
        {
            uint8_t mode = cpu.ax & 0xFF;
            cout << "INT10 Set Mode " << hex << (int)mode << endl;

            videoMode = mode;
            cpu.flags &= ~1;
            break;
        }

        case 0x0F:      // Get current video mode
        {
            cpu.ax = (80 << 8) | videoMode;   // AH=80 columns, AL=mode
            break;
        }

        case 0x0E:      // Teletype output
        {
            putchar(cpu.ax & 0xFF);
            fflush(stdout);
            break;
        }
case 0x12:
            printf("INT10 AH=12 AL=%02X BL=%02X\n",
                   cpu.ax & 0xff,
                   cpu.bx & 0xff);

            cpu.bx = 0;
            cpu.flags &= ~1;
            break;
        default:
            cout << "INT10 AH=" << hex << (int)ah
                 << " not implemented\n";
            break;
    }

    break;
}

        case 0x16:
            cout << "BIOS Keyboard Interrupt" << endl;
            break;


        case 0x13:
        {
            uint8_t ah = cpu.ax >> 8;

            fstream disk(
                "disk.img",
                ios::in | ios::out | ios::binary
            );

            if(!disk)
            {
                cout << "disk.img missing\n";
                cpu.flags |= 1;
                break;
            }


            switch(ah)
            {
                case 0x00: // reset
                    cout << "INT 13 Reset\n";
                    cpu.flags &= ~1;
                    break;
case 0x05: // Write sectors (CHS)
{
    uint8_t count = cpu.ax & 0xff;

    uint16_t cylinder = cpu.cx >> 8;
    uint16_t sector   = cpu.cx & 0x3f;
    uint16_t head     = cpu.dx >> 8;

    uint32_t lba =
        ((cylinder * 2 + head) * 18) +
        (sector - 1);

    uint32_t addr =
        ((uint32_t)cpu.es << 4) + cpu.bx;

    disk.seekp(lba * 512);

    disk.write(
        (char*)&memory[addr],
        count * 512
    );

    cout << "INT13 Write CHS "
         << (int)count
         << " sector(s)\n";

    cpu.ax &= 0x00FF;   // AH=0 success
    cpu.flags &= ~1;    // CF clear

    break;
}

                case 0x02: // read sectors
                {
                    uint8_t count = cpu.ax & 0xff;

                    uint16_t cylinder = (cpu.cx >> 8);
                    uint16_t sector = cpu.cx & 0x3f;
                    uint16_t head = cpu.dx >> 8;

                    uint32_t lba =
                        ((cylinder * 2 + head) * 18)
                        + (sector - 1);


                    uint32_t addr =
                        ((uint32_t)cpu.es << 4)
                        + cpu.bx;


                    disk.seekg(lba * 512);

                    disk.read(
                        (char*)&memory[addr],
                        count * 512
                    );


                    cout << "INT13 Read "
                         << (int)count
                         << " sector(s)\n";
cpu.ax &= 0x00FF;  // AH = 0
cpu.flags &= ~1;   // clear carry

                    
                    break;
                }


                case 0x03: // write sectors
                {
                    uint8_t count = cpu.ax & 0xff;


                    uint16_t cylinder = cpu.cx >> 8;
                    uint16_t sector = cpu.cx & 0x3f;
                    uint16_t head = cpu.dx >> 8;


                    uint32_t lba =
                        ((cylinder * 2 + head) * 18)
                        + (sector - 1);


                    uint32_t addr =
                        ((uint32_t)cpu.es << 4)
                        + cpu.bx;


                    disk.seekp(lba * 512);

                    disk.write(
                        (char*)&memory[addr],
                        count * 512
                    );


                    cout << "INT13 Write "
                         << (int)count
                         << " sector(s)\n";


                    cpu.ax &= 0x00FF;  // AH = 0
cpu.flags &= ~1;   // clear carry
                    break;
                }


                case 0x08: // get parameters
                    cpu.cx = 0x4F12;
                    cpu.dx = 0x0001;
                    cpu.flags &= ~1;
                    break;


                default:
                    cout << "INT13 AH="
                         << hex << (int)ah
                         << " not implemented\n";

                    cpu.flags |= 1;
                    break;
            }

            break;
        }


        default:
            cout << "Unknown BIOS INT "
                 << hex << (int)number
                 << endl;
    }
}
