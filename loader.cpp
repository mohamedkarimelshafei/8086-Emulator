#include "loader.h"
#include "cpu.h"
#include <fstream>
#include <iostream>

using namespace std;

bool LoadCOM(const char *filename)
{
    ifstream file(filename, ios::binary);

    if(!file)
        return false;

    uint32_t addr = Address(cpu.cs, 0x100);

    file.read((char *)&memory[addr], 65536);

    cout << "Loaded at: "
         << hex
         << addr
         << endl;

    cout << "First bytes: "
         << hex
         << (int)memory[addr] << " "
         << (int)memory[addr+1] << " "
         << (int)memory[addr+2] << endl;

    return true;
}
