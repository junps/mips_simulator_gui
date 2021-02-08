#include "simulator.h"
#include "simu_functions.h"
#include "instruction.h"

Simulator* create_simu(uint32_t pc, uint32_t sp) {
    Simulator* simu = (Simulator*)malloc(sizeof(Simulator));
    simu->mode = Normal;
    simu->text_field0 = (uint8_t*)malloc(TEXT_SIZE);
    simu->text_field1 = (uint8_t*)malloc(TEXT_SIZE);
    simu->text_field2 = (uint8_t*)malloc(TEXT_SIZE);
    simu->text_field3 = (uint8_t*)malloc(TEXT_SIZE);
    simu->data_field = (uint8_t*)malloc(DATA_SIZE);
    memset(simu->data_field, 0, DATA_SIZE);
    memset(simu->text_field0, 0, TEXT_SIZE);
    memset(simu->text_field1, 0, TEXT_SIZE);
    memset(simu->text_field2, 0, TEXT_SIZE);
    memset(simu->text_field3, 0, TEXT_SIZE);
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

//debug
void printbit(uint64_t num, int j) {
    for (int i=j-1; i>=0; i--) {
        if ((num >> i) & 0b1) {
            printf("1");
        }
        else printf("0");
    }
    printf("\n");
}

void execOneInstruction(Simulator* simu) {
    /* printf("gc:%d, gd%d\n", simu->gc, simu->gd); */
    for (int i=0; i<THREAD_NUM; i++) {
        int templ = get_template(simu, i);
        for (int j=0; j<3; j++) {
            uint32_t opcode = get_opcode(simu, i, j);
            uint32_t funct = get_func(simu, i, j);
            uint32_t fmt = get_fmt(simu, i, j);

            //debug
            uint64_t instr = ret_inst_64bit(simu, i, j);
            printbit(instr, 41);
            printf("opcode:");
            printbit(opcode, 6);

            if (instructions[opcode][funct][fmt] == NULL) {
                printf("\n\nNot Implemented: opcode : %x, funct : %x\n", opcode, funct);
                printf("pc is %d\n", simu->pc[0] / 4);
                exit(1);
            }

            instructions[opcode][funct][fmt](simu, i, j);

            //debug
            printf("pc:%x\n", simu->pc[i]);

        }
        switch (templ) {
            case 4:
            case 6:
            case 7:
            case 8:
                break;
            default:
                simu->pc[i] += 4;
                break;
        }
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
    free(simu->text_field0);
    free(simu->text_field1);
    free(simu->text_field2);
    free(simu->text_field3);
    free(simu->data_field);
    for (int i=0; i<THREAD_NUM; i++) {
        free(simu->stack_field[i]);
    }
    free(simu);
}

