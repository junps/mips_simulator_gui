#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "simulator.h"

void init_instructions(void);

typedef void instruction_func_t(Simulator*);

extern instruction_func_t* instructions[64][64][8];

#endif // INSTRUCTION_H
