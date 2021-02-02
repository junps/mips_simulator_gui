#include "simulator.h"
#include "simu_functions.h"
#include "instruction.h"

Simulator* create_simu(uint32_t pc, uint32_t sp) {
    Simulator* simu = (Simulator*)malloc(sizeof(Simulator));
    simu->mode = Normal;
    simu->text_field = (uint8_t*)malloc(TEXT_SIZE);
    simu->data_field = (uint8_t*)malloc(DATA_SIZE);
    memset(simu->data_field, 0, DATA_SIZE);
    memset(simu->text_field, 0, TEXT_SIZE);
    simu->gc = 0;
    simu->gd = 0;

    for (int i=0; i<THREAD_NUM; i++) {
        simu->stack_field[i] = (uint8_t*)malloc(STACK_SIZE);
        memset(simu->registers[i], 0, sizeof(simu->registers[0]));
        memset(simu->registers_f[i], 0, sizeof(simu->registers[0]));
        memset(simu->condition_code[i], 0, sizeof(simu->condition_code[0]));
        memset(simu->stack_field[i], 0, STACK_SIZE);

        simu->pc[i] = pc;
        simu->registers[i][SP] = sp;
        simu->registers[i][ZERO] = 0;
    }

    return simu;
}

void execOneInstruction(Simulator* simu) {
    /* printf("gc:%d, gd%d\n", simu->gc, simu->gd); */
    for (int i=0; i<THREAD_NUM; i++) {
        uint32_t opcode = get_opcode(simu, i);
        uint32_t funct = get_func(simu, i);
        uint32_t fmt = get_fmt(simu, i);

//        printf("opcode : %d, funct : %d\n", opcode, funct);

        if (instructions[opcode][funct][fmt] == NULL) {
            printf("\n\nNot Implemented: opcode : %x, funct : %x\n", opcode, funct);
            printf("pc is %d\n", simu->pc[0] / 4);
            exit(1);
        }

        instructions[opcode][funct][fmt](simu, i);

        if(simu->mode == Normal) break;

        /* printf("thread:%d, pc:%d\n", i, simu->pc[i]); */
    }
}

void memory_dump(Simulator *simu) {
    FILE *dump = fopen("./memory_dump.txt", "w");
    for (int i=0; i<DATA_SIZE; i++) {
        fprintf(dump, "%d: %x\n", i+1, simu->data_field[i]);
    }
    fclose(dump);
}

void destroy_simu(Simulator* simu) {
    free(simu->text_field);
    free(simu->data_field);
    for (int i=0; i<THREAD_NUM; i++) {
        free(simu->stack_field[i]);
    }
    free(simu);
}

