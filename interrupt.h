#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <cstdint>

struct IntVector
{
    uint16_t offset;
    uint16_t segment;
};

extern IntVector ivt[256];

#endif
