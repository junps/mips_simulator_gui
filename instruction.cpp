#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <QDebug>

#include "instruction.h"
#include "simulator.h"
#include "simu_functions.h"

instruction_func_t* instructions[64][64];

void add(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    uint32_t rd = get_rd(simu);
    simu->registers[rd] = simu->registers[rs] + simu->registers[rt];
    simu->pc += 4;
}

void addi(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    simu->registers[rt] = simu->registers[rs] + imm;
    simu->pc += 4;
}

static void lw(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    uint32_t num = ret_32bit(simu, simu->registers[rs] + imm);
    simu->registers[rt] = num;
    simu->pc += 4;
}

static void lui(Simulator* simu) {
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    int32_t num = (imm << 16) & 0xffff;
    simu->registers[rt] = num;
    simu->pc += 4;
}

static void or_f(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    uint32_t rd = get_rd(simu);
    simu->registers[rd] = (simu->registers[rt]) | (simu->registers[rs]);
    simu->pc += 4;
}

static void ori(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    simu->registers[rt] = imm | (simu->registers[rs]);
    simu->pc += 4;
}

static void sw(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    divide_8bits_store(simu, simu->registers[rs] + imm, simu->registers[rt]);
    simu->pc += 4;
}

static void slt(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    uint32_t rd = get_rd(simu);
    int new_num = 0;
    if(simu->registers[rs] < simu->registers[rt]) new_num = 1;
    simu->registers[rd] = new_num;
    simu->pc += 4;
}

static void slti(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    int new_num = 0;
    if(simu->registers[rs] < imm) new_num = 1;
    simu->registers[rt] = new_num;
    simu->pc += 4;
}

static void bne(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    if(simu->registers[rs] != simu->registers[rt]) {
        simu->pc += (imm << 2) + 4;
    } else {
        simu->pc += 4;
    }
}

static void beq(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    if(simu->registers[rs] == simu->registers[rt]) {
        simu->pc += (imm << 2) + 4;
    } else {
        simu->pc += 4;
    }
}

static void j(Simulator* simu) {
    uint32_t addr = get_address(simu);
    simu->pc = (simu->pc & 0xf0000000) | (addr << 2);
}

static void jr(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    simu->pc = simu->registers[rs];
}

static void jal(Simulator* simu) {
    uint32_t addr = get_address(simu);
    simu->registers[31] = simu->pc + 4;
    simu->pc = (simu->pc & 0xf0000000) | (addr << 2);
}

static void set_inst(uint32_t opcode, uint32_t funct, int unique, instruction_func_t* inst) {
    if(unique == 1) {
        for(int i = 0; i < 64; i++) instructions[opcode][i] = inst;
    } else {
        instructions[opcode][funct] = inst;
    }
}

void init_instructions(void) {
    memset(instructions, 0, sizeof(instructions));
    uint32_t dummybit = 0b000000;

    set_inst(0b000000, 0b100000, 0, &add);
    set_inst(0b001000, dummybit, 1, &addi);
    set_inst(0b100011, dummybit, 1, &lw);
    set_inst(0b001111, dummybit, 1, &lui);
    set_inst(0b000000, 0b100101, 0, &or_f);
    set_inst(0b001101, dummybit, 1, &ori);
    set_inst(0b101011, dummybit, 1, &sw);
    set_inst(0b000000, 0b101010, 0, &slt);
    set_inst(0b001010, dummybit, 1, &slti);
    set_inst(0b000101, dummybit, 1, &bne);
    set_inst(0b000100, dummybit, 1, &beq);
    set_inst(0b000010, dummybit, 1, &j);
    set_inst(0b000000, 0b001000, 0, &jr);
    set_inst(0b000011, dummybit, 1, &jal);
}
