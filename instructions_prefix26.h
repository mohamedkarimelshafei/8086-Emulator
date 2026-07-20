#ifndef INSTRUCTIONS_PREFIX26_H
#define INSTRUCTIONS_PREFIX26_H

#include "cpu.h"

extern bool segmentOverride;
extern uint16_t overrideSegment;

void Prefix26();

#endif
