#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>

#define REGISTER_NUM 32

enum Register { ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7,
                S0, S1, S2, S3, S4, S5, S6, S7, T8, T9, K0, K1, GP, SP, FP, RA };

typedef struct {
    int32_t registers[REGISTER_NUM];

    uint8_t* memory;

    uint32_t pc; //program counter
} Simulator;

#endif // SIMULATOR_H
