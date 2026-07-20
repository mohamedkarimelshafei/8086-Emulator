#include "kernel.h"

DOSKernel::DOSKernel(CPU& c, Memory& m)
    : cpu(c), memory(m)
{
}

bool DOSKernel::Initialize()
{
    return true;
}
