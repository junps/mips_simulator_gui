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
#define THREAD_NUM 10

#define TEXT_SIZE 50000
#define DATA_SIZE 524288
#define STACK_SIZE 60000

enum Register { ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7,
                S0, S1, S2, S3, S4, S5, S6, S7, T8, T9, K0, K1, GP, SP, FP, RA };

enum Register_f {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11,
                 F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22,
                 F23, F24, F25, F26, F27, F28, F29, F30, F31};

enum Mode {
    Normal, Parallel
};

typedef struct {
    Mode mode;

    int32_t registers[THREAD_NUM][REGISTER_NUM];
    float registers_f[THREAD_NUM][REGISTER_NUM];
    int gc;
    int gd;
    int condition_code[THREAD_NUM][8];

    uint8_t* text_field;
    uint8_t* data_field;
    uint8_t* stack_field[THREAD_NUM];

    uint32_t pc[THREAD_NUM]; //program counter
} Simulator;

struct Small_simu{
    Mode mode;

    int32_t registers[THREAD_NUM][REGISTER_NUM];
    float registers_f[THREAD_NUM][REGISTER_NUM];
    int condition_code[THREAD_NUM][8];

    uint8_t* data;
    uint8_t* stack[THREAD_NUM];

    uint32_t pc[THREAD_NUM]; //program counter

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
    int stack_size = STACK_SIZE;
    int data_size = DATA_SIZE;

    Small_simu *create_initial() {
        Small_simu* small_simu = (Small_simu*)malloc(sizeof(Small_simu));
        small_simu->data = (uint8_t*)malloc(DATA_SIZE + 1);
        memset(small_simu->data, 0, sizeof(uint8_t) * DATA_SIZE);

        for (int i=0; i<THREAD_NUM; i++) {
            small_simu->stack[i] = (uint8_t*)malloc(STACK_SIZE + 1);
            memset(small_simu->stack[i], 0, sizeof(uint8_t) * STACK_SIZE);
            memset(small_simu->registers[i], 0, sizeof(small_simu->registers[0]));
            memset(small_simu->registers_f[i], 0, sizeof(small_simu->registers_f[0]));
            memset(small_simu->condition_code[i], 0, sizeof(small_simu->condition_code[0]));
            small_simu->pc[i] = 0;
        }

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

        for(int j=0; j<THREAD_NUM; j++) {
            for(int i = 0; i < 32; i++) {
                new_simu->registers[j][i] = simu->registers[j][i];
            }

            for(int i = 0; i < 32; i++) {
                new_simu->registers_f[j][i] = simu->registers_f[j][i];
            }

            for(int i = 0; i < 8; i++) {
                new_simu->condition_code[j][i] = simu->condition_code[j][i];
            }

            new_simu->pc[j] = simu->pc[j];

            for(int i = 0; i <= STACK_SIZE; i++) {
                new_simu->stack[j][i] = simu->stack_field[j][ini_sp + i];
            }
        }

        for(int i = 0; i <= DATA_SIZE; i++) {
            new_simu->data[i] = simu->data_field[ini_sp + i];
        }
    }

    void delete_whole_l() {
        now_node = boss;
        while(1) {
            if(boss->next == boss) {
                return;
            } else if(boss->next->next == boss) {
                free(boss->next->data);
                free(boss->next->stack);
                free(boss->next);
                break;
            }
            boss->next = boss->next->next;
            free(boss->next->prev->data);
            free(boss->next->prev->stack);
            free(boss->next->prev);
            boss->next->prev = boss;
        }
        boss->next = boss;
        boss->prev = boss;
        siz = 0;
    }

    void change_simu(Simulator* simu) {
        for (int j=0; j<THREAD_NUM; j++) {
            for(int i = 0; i < 32; i++) {
                simu->registers[j][i] = now_node->registers[j][i];
            }

            for(int i = 0; i < 32; i++) {
                simu->registers_f[j][i] = now_node->registers_f[j][i];
            }

            for(int i = 0; i < 8; i++) {
                simu->condition_code[j][i] = now_node->condition_code[j][i];
            }

            simu->pc[j] = now_node->pc[j];

            for(int i = 0; i <= STACK_SIZE; i++) {
                simu->stack_field[j][ini_sp + i] = now_node->stack[j][i];
            }
        }

        for(int i = 0; i <= DATA_SIZE; i++) {
            simu->data_field[ini_sp + i] = now_node->data[i];
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

// functions
Simulator* create_simu(uint32_t pc, uint32_t sp);
void execOneInstruction(Simulator* simu);
void memory_dump(Simulator *simu);
void destroy_simu(Simulator* simu);

#endif // SIMULATOR_H
