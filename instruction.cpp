#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cmath>
#include <QDebug>

#include "instruction.h"
#include "simulator.h"
#include "simu_functions.h"

instruction_func_t* instructions[64][64][8];

FILE *fp_out;
//QTextStream out;

QString output_file = "output.txt";
QFile file(output_file);

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

void sub(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    uint32_t rd = get_rd(simu);
    simu->registers[rd] = simu->registers[rs] - simu->registers[rt];
    simu->pc += 4;
}

static void lw(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    int32_t num = ret_32bit(simu, simu->registers[rs] + imm);
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

static void and_f(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    uint32_t rd = get_rd(simu);
    simu->registers[rd] = (simu->registers[rt]) & (simu->registers[rs]);
    simu->pc += 4;
}

static void ori(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    simu->registers[rt] = imm | (simu->registers[rs]);
    simu->pc += 4;
}

static void andi(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    int32_t imm = get_imm(simu);
    simu->registers[rt] = imm & (simu->registers[rs]);
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

static void sll(Simulator* simu) {
    uint32_t rt = get_rt(simu);
    uint32_t rd = get_rd(simu);
    int32_t imm = get_imm(simu);
    simu->registers[rd] = simu->registers[rt] << ((imm >> 6) & 0b11111);
    simu->pc += 4;
}

static void srl(Simulator* simu) {
    uint32_t rt = get_rt(simu);
    uint32_t rd = get_rd(simu);
    int32_t imm = get_imm(simu);
    simu->registers[rd] = simu->registers[rt] >> ((imm >> 6) & 0b11111);
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

static void in_f(Simulator* simu) {
    uint32_t rt = get_rt(simu);
    simu->registers[rt] = ((simu->registers[rt] >> 8) << 8) | 0b1010;
    simu->pc += 4;
}

static void out_f(Simulator* simu) {
    uint32_t rt = get_rt(simu);
    char buf[100];
    sprintf(buf, "%x", (simu->registers[rt] & 0xff));
    //fwrite(buf, 2, sizeof(char), fp_out);
    //fputc('\n', fp_out);

    QTextStream out(&file);
    out << buf;
    simu->pc += 4;
}

void bt_s(Simulator* simu) {
    uint32_t rt = get_rt(simu);
    uint32_t cc = (rt >> 2) & 0b111;
    int32_t imm = get_imm(simu);
    if(simu->condition_code[cc] == 1) {
        simu->pc += (imm << 2) + 4;
    } else {
        simu->pc += 4;
    }
}

void bf_s(Simulator* simu) {
    uint32_t rt = get_rt(simu);
    uint32_t cc = (rt >> 2) & 0b111;
    int32_t imm = get_imm(simu);
    if(simu->condition_code[cc] == 0) {
        simu->pc += (imm << 2) + 4;
    } else {
        simu->pc += 4;
    }
}

void add_s(Simulator* simu) {
    uint32_t fs = get_rd(simu);
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);

    union Single a, b;
    a.f = simu->registers_f[fs];
    b.f = simu->registers_f[ft];
    simu->registers_f[fd] = fadd(a, b).f;
    //simu->registers_f[fd] = simu->registers_f[fs] + simu->registers_f[ft];
    simu->pc += 4;
}

void sub_s(Simulator* simu) {
    uint32_t fs = get_rd(simu);
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);

    union Single a, b;
    a.f = simu->registers_f[fs];
    b.f = simu->registers_f[ft];
    simu->registers_f[fd] = fsub(a, b).f;
    //simu->registers_f[fd] = simu->registers_f[fs] - simu->registers_f[ft];
    simu->pc += 4;
}

void mul_s(Simulator* simu) {
    uint32_t fs = get_rd(simu);
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);

    union Single a, b;
    a.f = simu->registers_f[fs];
    b.f = simu->registers_f[ft];
    simu->registers_f[fd] = fmul(a, b).f;
    //simu->registers_f[fd] = simu->registers_f[fs] * simu->registers_f[ft];
    simu->pc += 4;
}

void div_s(Simulator* simu) {
    uint32_t fs = get_rd(simu);
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);

    union Single a, b;
    a.f = simu->registers_f[fs];
    b.f = simu->registers_f[ft];
    simu->registers_f[fd] = fdiv(a, b).f;
    //simu->registers_f[fd] = simu->registers_f[fs] / simu->registers_f[ft];
    simu->pc += 4;
}

void mov_s(Simulator* simu) {
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);
    simu->registers_f[fd] = simu->registers_f[ft];
    simu->pc += 4;
}

void neg_s(Simulator* simu) {
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);
    simu->registers_f[fd] = -simu->registers_f[ft];
    simu->pc += 4;
}

void abs_s(Simulator* simu) {
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);
    simu->registers_f[fd] = abs(simu->registers_f[ft]);
    simu->pc += 4;
}

void sqrt_s(Simulator* simu) {
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);

    union Single a;
    a.f = simu->registers_f[ft];
    simu->registers_f[fd] = fsqrt(a).f;
    //simu->registers_f[fd] = sqrt(simu->registers_f[ft]);
    simu->pc += 4;
}

void lw_s(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t ft = get_rt(simu);
    int32_t imm = get_imm(simu);
    int32_t num = ret_32bit(simu, simu->registers[rs] + imm);
    union { float f; int i; } f_and_i;
    f_and_i.i = num;
    simu->registers_f[ft] = f_and_i.f;
    simu->pc += 4;
}

void sw_s(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t ft = get_rt(simu);
    int32_t imm = get_imm(simu);
    union { float f; int i; } f_and_i;
    f_and_i.f = simu->registers_f[ft];
    divide_8bits_store(simu, simu->registers[rs] + imm, f_and_i.i);
    simu->pc += 4;
}

void ftoi(Simulator* simu) {
    uint32_t fs = get_rs(simu);
    uint32_t rt = get_rt(simu);
    simu->registers[rt] = (int)simu->registers_f[fs];
    simu->pc += 4;
}

void itof(Simulator* simu) {
    uint32_t rs = get_rs(simu);
    uint32_t ft = get_rt(simu);
    simu->registers_f[ft] = (float)simu->registers[rs];
    simu->pc += 4;
}

void c_eq_s(Simulator* simu) {
    uint32_t fs = get_rd(simu);
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);
    uint32_t cc = (fd >> 2) & 0b111;
    if (simu->registers_f[fs] == simu->registers_f[ft]) {
        simu->condition_code[cc] = 1;
    } else {
        simu->condition_code[cc] = 0;
    }
    simu->pc += 4;
}

void c_lt_s(Simulator* simu) {
    uint32_t fs = get_rd(simu);
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);
    uint32_t cc = (fd >> 2) & 0b111;
    if (simu->registers_f[fs] < simu->registers_f[ft]) {
        simu->condition_code[cc] = 1;
    } else {
        simu->condition_code[cc] = 0;
    }
    simu->pc += 4;
}

void c_le_s(Simulator* simu) {
    uint32_t fs = get_rd(simu);
    uint32_t ft = get_rt(simu);
    uint32_t fd = ((get_imm(simu) >> 6) & 0b11111);
    uint32_t cc = (fd >> 2) & 0b111;
    if (simu->registers_f[fs] <= simu->registers_f[ft]) {
        simu->condition_code[cc] = 1;
    } else {
        simu->condition_code[cc] = 0;
    }
    simu->pc += 4;
}

static void set_inst(uint32_t opcode, uint32_t funct, uint32_t fmt, int unique, instruction_func_t* inst) {
    if(unique == 1) {
        for(int i = 0; i < 64; i++) {
            for (int j = 0; j < 8; j++) {
                instructions[opcode][i][j] = inst;
            }
        }
    } else if(unique == 0){
        for (int i = 0; i < 8; i++) {
            instructions[opcode][funct][i] = inst;
        }
    } else if (unique == 2){
        for (int i = 0; i < 64; i++) {
            instructions[opcode][i][fmt] = inst;
        }
    } else {
        instructions[opcode][funct][fmt] = inst;
        instructions[opcode][funct][(fmt | 0b1)] = inst;
    }
}

void init_instructions(void) {
    memset(instructions, 0, sizeof(instructions));
    uint32_t dummybit = 0b000000;
    uint32_t dmfmt = 0b000;

    set_inst(0b000000, 0b100000, dmfmt, 0, &add);
    set_inst(0b000000, 0b100010, dmfmt, 0, &sub);
    set_inst(0b001000, dummybit, dmfmt, 1, &addi);
    set_inst(0b100011, dummybit, dmfmt, 1, &lw);
    set_inst(0b001111, dummybit, dmfmt, 1, &lui);
    set_inst(0b000000, 0b100101, dmfmt, 0, &or_f);
    set_inst(0b000000, 0b100100, dmfmt, 0, &and_f);
    set_inst(0b001101, dummybit, dmfmt, 1, &ori);
    set_inst(0b001100, dummybit, dmfmt, 1, &andi);
    set_inst(0b101011, dummybit, dmfmt, 1, &sw);
    set_inst(0b000000, 0b101010, dmfmt, 0, &slt);
    set_inst(0b001010, dummybit, dmfmt, 1, &slti);
    set_inst(0b000000, 0b000000, dmfmt, 0, &sll);
    set_inst(0b000000, 0b000010, dmfmt, 0, &srl);
    set_inst(0b000101, dummybit, dmfmt, 1, &bne);
    set_inst(0b000100, dummybit, dmfmt, 1, &beq);
    set_inst(0b000010, dummybit, dmfmt, 1, &j);
    set_inst(0b000000, 0b001000, dmfmt, 0, &jr);
    set_inst(0b000011, dummybit, dmfmt, 1, &jal);
    set_inst(0b011010, dummybit, dmfmt, 1, &in_f);
    set_inst(0b011011, dummybit, dmfmt, 1, &out_f);

    set_inst(0b010001, dummybit, 0b011, 2, &bt_s);
    set_inst(0b010001, dummybit, 0b010, 2, &bf_s);
    set_inst(0b010001, 0b000000, 0b100, 3, &add_s);
    set_inst(0b010001, 0b000001, 0b100, 3, &sub_s);
    set_inst(0b010001, 0b000010, 0b100, 3, &mul_s);
    set_inst(0b010001, 0b000011, 0b100, 3, &div_s);
    set_inst(0b010001, 0b000110, 0b100, 3, &mov_s);
    set_inst(0b010001, 0b000111, 0b100, 3, &neg_s);
    set_inst(0b010001, 0b000101, 0b100, 3, &abs_s);
    set_inst(0b010001, 0b000100, 0b100, 3, &sqrt_s);
    set_inst(0b010001, 0b110010, 0b100, 3, &c_eq_s);
    set_inst(0b010001, 0b111100, 0b100, 3, &c_lt_s);
    set_inst(0b010001, 0b111110, 0b100, 3, &c_le_s);
    set_inst(0b110001, dummybit, dmfmt, 1, &lw_s);
    set_inst(0b111001, dummybit, dmfmt, 1, &sw_s);
    set_inst(0b111000, dummybit, dmfmt, 1, &ftoi);
    set_inst(0b110000, dummybit, dmfmt, 1, &itof);


    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "cannot open output.txt";
        return;
    }
}
