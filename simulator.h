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

using namespace std;

#define REGISTER_NUM 32

enum Register { ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7,
                S0, S1, S2, S3, S4, S5, S6, S7, T8, T9, K0, K1, GP, SP, FP, RA };

typedef struct {
    int32_t registers[REGISTER_NUM];

    uint8_t* memory;

    uint32_t pc; //program counter
} Simulator;

struct Small_simu{
    int32_t registers[REGISTER_NUM];

    uint8_t* stack;

    uint32_t pc; //program counter

    struct Small_simu *prev;
    struct Small_simu *next;
};

class Simu_linked_list {
public:
    Small_simu *boss;
    Small_simu *now_node;
    int siz;
    int mx_siz = 10000;
    int ini_sp = 4096;

    Small_simu *create_initial() {
        Small_simu* small_simu = (Small_simu*)malloc(sizeof(Small_simu));
        small_simu->stack = (uint8_t*)malloc(ini_sp);

        memset(small_simu->registers, 0, sizeof(small_simu->registers));

        small_simu->pc = 0;

        small_simu->prev = NULL;
        small_simu->next = NULL;

        return small_simu;
    }

    void create_new(Simulator *simu) {
        cout << "1" << endl;
        Small_simu* new_simu = create_initial();

        new_simu->prev = boss->prev;
        boss->prev->next = new_simu;
        boss->prev = new_simu;
        new_simu->next = boss;
        now_node = new_simu;

        for(int i = 0; i < 32; i++) {
            new_simu->registers[i] = simu->registers[i];
        }

        new_simu->pc = simu->pc;

        for(int i = 0; i <= ini_sp; i++) {
            new_simu->stack[ini_sp - i] = simu->memory[ini_sp - i];
        }
    }

    void delete_whole_l() {
        now_node = boss;
        while(1) {
            if(boss->next == boss) {
                return;
            } else if(boss->next->next == boss) {
                free(boss->next->stack);
                free(boss->next);
                break;
            }
            boss->next = boss->next->next;
            free(boss->next->prev->stack);
            free(boss->next->prev);
            boss->next->prev = boss;
        }
        boss->next = boss;
        boss->prev = boss;
        siz = 0;
//        boss->next = boss;
//        boss->prev = boss;
//        now_node = boss;
//        siz = 0;
    }

    Simu_linked_list() {
        boss = create_initial();
        now_node = boss;
        boss->prev = boss;
        boss->next = boss;
        siz = 0;
    }
};

#endif // SIMULATOR_H
