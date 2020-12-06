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

enum Register_f {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11,
                 F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22,
                 F23, F24, F25, F26, F27, F28, F29, F30, F31};

typedef struct {
    int32_t registers[REGISTER_NUM];
    float registers_f[REGISTER_NUM];
    int condition_code[8];

    uint8_t* text_field;
    uint8_t* stack_field;

    uint32_t pc; //program counter
} Simulator;

struct Small_simu{
    int32_t registers[REGISTER_NUM];
    float registers_f[REGISTER_NUM];
    int condition_code[8];

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
    int mx_siz = 100;
    int ini_sp = 0;
    int stack_size = 1024 * 1024;

    Small_simu *create_initial() {
        Small_simu* small_simu = (Small_simu*)malloc(sizeof(Small_simu));
        small_simu->stack = (uint8_t*)malloc(stack_size + 1);
        memset(small_simu->stack, 0, sizeof(uint8_t) * stack_size);

        memset(small_simu->registers, 0, sizeof(small_simu->registers));
        memset(small_simu->registers_f, 0, sizeof(small_simu->registers_f));
        memset(small_simu->condition_code, 0, sizeof(small_simu->condition_code));

        small_simu->pc = 0;

        small_simu->prev = NULL;
        small_simu->next = NULL;

        return small_simu;
    }

    void create_new(Simulator *simu) {
        Small_simu* new_simu = create_initial();

        new_simu->prev = boss->prev;
        boss->prev->next = new_simu;
        boss->prev = new_simu;
        new_simu->next = boss;
        now_node = new_simu;

        for(int i = 0; i < 32; i++) {
            new_simu->registers[i] = simu->registers[i];
        }

        for(int i = 0; i < 32; i++) {
            new_simu->registers_f[i] = simu->registers_f[i];
        }

        for(int i = 0; i < 8; i++) {
            new_simu->condition_code[i] = simu->condition_code[i];
        }

        new_simu->pc = simu->pc;

        for(int i = 0; i <= stack_size; i++) {
            new_simu->stack[i] = simu->stack_field[ini_sp + i];
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
    }

    void change_simu(Simulator* simu) {
        for(int i = 0; i < 32; i++) {
            simu->registers[i] = now_node->registers[i];
        }

        for(int i = 0; i < 32; i++) {
            simu->registers_f[i] = now_node->registers_f[i];
        }

        for(int i = 0; i < 8; i++) {
            simu->condition_code[i] = now_node->condition_code[i];
        }

        simu->pc = now_node->pc;

        for(int i = 0; i <= stack_size; i++) {
            simu->stack_field[ini_sp + i] = now_node->stack[i];
        }

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
