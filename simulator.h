#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <map>
#include <bitset>

#define REGISTER_NUM 32

enum Register { ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7,
                S0, S1, S2, S3, S4, S5, S6, S7, T8, T9, K0, K1, GP, SP, FP, RA };

typedef struct {
    int32_t registers[REGISTER_NUM];

    uint8_t* memory;

    uint32_t pc; //program counter
} Simulator;

//struct Small_simu{
//    int32_t registers[REGISTER_NUM];

//    uint8_t* stack;

//    uint32_t pc; //program counter

//    struct Small_simu *prev;
//    struct Small_simu *next;
//};

//class Simu_linked_list {
//public:
//    Small_simu *boss;
//    Small_simu *now_node;
//    int siz;
//    int ini_sp = 4096;

//    Small_simu *create_small_simu() {
//        Small_simu* small_simu = (Small_simu*)malloc(sizeof(Small_simu));
//        small_simu->stack = (uint8_t*)malloc(ini_sp);

//        memset(small_simu->registers, 0, sizeof(small_simu->registers));

//        small_simu->pc = 0;

//        small_simu->prev = NULL;
//        small_simu->next = NULL;

//        return small_simu;
//    }

//    Simu_linked_list() {
//        boss = create_small_simu();
//        now_node = boss;
//        boss->prev = boss;
//        boss->next = boss;
//        siz = 0;
//    }
//};

#endif // SIMULATOR_H
