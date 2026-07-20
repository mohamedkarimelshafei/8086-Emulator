#pragma once

class CPU;
class Memory;

class DOSKernel
{
public:
    DOSKernel(CPU& cpu, Memory& memory);

    bool Initialize();

    void Int20();
    void Int21();

private:
    CPU& cpu;
    Memory& memory;
};
