#include "bios.h"
#include "cpu.h"
#include <iostream>

using namespace std;

static uint8_t currentMode = 3;

void BIOSVideo()
{
    BIOSInterrupt(0x10);

}
