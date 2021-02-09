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

//QString output_file = "output.ppm";
//QFile file(output_file);

FILE *fp_out;

void nop(Simulator* simu, int t, int unit) {
    return;
}

void add(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t rd = get_rd(simu, t, unit);
    simu->registers[t][rd] = simu->registers[t][rs] + simu->registers[t][rt];
    /* simu->pc[t] += 4; */
}

void addi(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    simu->registers[t][rt] = simu->registers[t][rs] + imm;
    /* simu->pc[t] += 4; */
}

void sub(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t rd = get_rd(simu, t, unit);
    simu->registers[t][rd] = simu->registers[t][rs] - simu->registers[t][rt];
    /* simu->pc[t] += 4; */
}

static void lw(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    /* int32_t num = ret_32bit(simu, t, simu->registers[t][rs] + imm); */
    int32_t num;
    uint32_t from = simu->registers[t][rs] + imm;
    if (simu->mode == Parallel) {
        Spec *s;
        for (s = simu->spec_list[t]; s != NULL && s->addr != from; s = s->next);
        if (s == NULL) {
            simu->registers[t][rt] = ret_32bit(simu, t, from);
            return;
        } else {
            simu->registers[t][rt] = s->data;
            return;
        }
    } else {
        simu->registers[t][rt] = ret_32bit(simu, t, from);
        return;
    }
    /* simu->pc[t] += 4; */
}

static void lui(Simulator* simu, int t, int unit) {
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t imm = get_uimm(simu, t, unit);
    uint32_t num = imm << 16;
    simu->registers[t][rt] = num;
    /* simu->pc[t] += 4; */
}

static void or_f(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t rd = get_rd(simu, t, unit);
    simu->registers[t][rd] = (simu->registers[t][rt]) | (simu->registers[t][rs]);
    /* simu->pc[t] += 4; */
}

static void and_f(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t rd = get_rd(simu, t, unit);
    simu->registers[t][rd] = (simu->registers[t][rt]) & (simu->registers[t][rs]);
    /* simu->pc[t] += 4; */
}

static void ori(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t imm = get_uimm(simu, t, unit);
    simu->registers[t][rt] = imm | (simu->registers[t][rs]);
    /* simu->pc[t] += 4; */
}

static void andi(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t imm = get_uimm(simu, t, unit);
    simu->registers[t][rt] = imm & (simu->registers[t][rs]);
    /* simu->pc[t] += 4; */
}

static void sw(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    divide_8bits_store(simu, t, simu->registers[t][rs] + imm, simu->registers[t][rt], simu->mode);
    /* simu->pc[t] += 4; */
}

static void slt(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t rd = get_rd(simu, t, unit);
    int new_num = 0;
    if(simu->registers[t][rs] < simu->registers[t][rt]) new_num = 1;
    simu->registers[t][rd] = new_num;
    /* simu->pc[t] += 4; */
}

static void slti(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    int new_num = 0;
    if(simu->registers[t][rs] < imm) new_num = 1;
    simu->registers[t][rt] = new_num;
    /* simu->pc[t] += 4; */
}

static void sll(Simulator* simu, int t, int unit) {
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t rd = get_rd(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    simu->registers[t][rd] = simu->registers[t][rt] << ((imm >> 6) & 0b11111);
    /* simu->pc[t] += 4; */
}

static void srl(Simulator* simu, int t, int unit) {
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t rd = get_rd(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    simu->registers[t][rd] = simu->registers[t][rt] >> ((imm >> 6) & 0b11111);
    /* simu->pc[t] += 4; */
}

static void bne(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    if(simu->registers[t][rs] != simu->registers[t][rt]) {
        simu->pc[t] += (imm << 2) + 4;
    } else {
        simu->pc[t] += 4;
    }
}

static void beq(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    if(simu->registers[t][rs] == simu->registers[t][rt]) {
        simu->pc[t] += (imm << 2) + 4;
    } else {
        simu->pc[t] += 4;
    }
}

static void j(Simulator* simu, int t, int unit) {
    uint32_t addr = get_address(simu, t, unit);
    /* simu->pc[t] = (simu->pc[t] & 0xf0000000) | (addr << 2); */
    //debug
    /* printf("addr:%x\n", addr); */
    simu->pc[t] = (addr << 2);
}

static void jr(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    simu->pc[t] = simu->registers[t][rs];
}

static void jal(Simulator* simu, int t, int unit) {
    uint32_t addr = get_address(simu, t, unit);
    simu->registers[t][31] = simu->pc[t] + 4;
    simu->pc[t] = (simu->pc[t] & 0xf0000000) | (addr << 2);
}

static void jalr(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    simu->registers[t][31] = simu->pc[t] + 4;
    simu->pc[t] = simu->registers[t][rs];
}

static void in_f(Simulator* simu, int t, int unit) {
    uint32_t rt = get_rt(simu, t, unit);
    //simu->registers[rt] = ((simu->registers[rt] >> 8) << 8) | 0b1010;
    simu->registers[t][rt] = read_a_sld_byte().i;
    /* simu->pc[t] += 4; */
}

int fst = 1;

static void out_f(Simulator* simu, int t, int unit) {
#if RAYTRACE
    if(fst) {
        fst = 0;
        /* simu->pc[t] += 4; */
    } else {
        uint32_t rt = get_rt(simu, t, unit);
        fwrite((char*)&(simu->registers[t][rt]), 1, 1, fp_out);
        //fwrite((char*)&(simu->registers[rt]), 1, 1, stdout);
        /* simu->pc[t] += 4; */
    }
#else
    uint32_t rt = get_rt(simu, t, unit);
    fwrite((char*)&(simu->registers[t][rt]), 1, 1, fp_out);
    //fwrite((char*)&(simu->registers[rt]), 1, 1, stdout);
    /* simu->pc[t] += 4; */
#endif
}

void bt_s(Simulator* simu, int t, int unit) {
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t cc = (rt >> 5) & 0b111;
    int32_t imm = get_imm(simu, t, unit);
    if(simu->condition_code[t][cc] == 1) {
        simu->pc[t] += (imm << 2) + 4;
    } else {
        simu->pc[t] += 4;
    }
}

void bf_s(Simulator* simu, int t, int unit) {
    uint32_t rt = get_rt(simu, t, unit);
    uint32_t cc = (rt >> 5) & 0b111;
    int32_t imm = get_imm(simu, t, unit);
    if(simu->condition_code[t][cc] == 0) {
        simu->pc[t] += (imm << 2) + 4;
    } else {
        simu->pc[t] += 4;
    }
}

void add_s(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rd(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);

#if NOT_FPU
    simu->registers_f[t][fd] = simu->registers_f[t][fs] + simu->registers_f[t][ft];
#else
    union Single a, b;
    a.f = simu->registers_f[t][fs];
    b.f = simu->registers_f[t][ft];
    simu->registers_f[t][fd] = fadd(a, b).f;
#endif
    /* simu->pc[t] += 4; */
}

void sub_s(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rd(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);

#if NOT_FPU
    simu->registers_f[t][fd] = simu->registers_f[t][fs] - simu->registers_f[t][ft];
#else
    union Single a, b;
    a.f = simu->registers_f[t][fs];
    b.f = simu->registers_f[t][ft];
    simu->registers_f[t][fd] = fsub(a, b).f;
#endif
    /* simu->pc[t] += 4; */
}

void mul_s(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rd(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);

#if NOT_FPU
    simu->registers_f[t][fd] = simu->registers_f[t][fs] * simu->registers_f[t][ft];
#else
    union Single a, b;
    a.f = simu->registers_f[t][fs];
    b.f = simu->registers_f[t][ft];
    simu->registers_f[t][fd] = fmul(a, b).f;
#endif
    /* simu->pc[t] += 4; */
}

void div_s(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rd(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);

#if NOT_FPU
    simu->registers_f[t][fd] = simu->registers_f[t][fs] / simu->registers_f[t][ft];
#else
    union Single a, b;
    a.f = simu->registers_f[t][fs];
    b.f = simu->registers_f[t][ft];
    simu->registers_f[t][fd] = fdiv(a, b).f;
#endif
    /* simu->pc[t] += 4; */
}

void mov_s(Simulator* simu, int t, int unit) {
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);
    simu->registers_f[t][fd] = simu->registers_f[t][ft];
    /* simu->pc[t] += 4; */
}

void neg_s(Simulator* simu, int t, int unit) {
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);
    simu->registers_f[t][fd] = -simu->registers_f[t][ft];
    /* simu->pc[t] += 4; */
}

void abs_s(Simulator* simu, int t, int unit) {
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);
    simu->registers_f[t][fd] = abs(simu->registers_f[t][ft]);
    /* simu->pc[t] += 4; */
}

void sqrt_s(Simulator* simu, int t, int unit) {
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);

#if NOT_FPU
    simu->registers_f[t][fd] = sqrt(simu->registers_f[t][ft]);
#else
    union Single a;
    a.f = simu->registers_f[t][ft];
    simu->registers_f[t][fd] = fsqrt(a).f;
#endif
    /* simu->pc[t] += 4; */
}

void lw_s(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    /* int32_t num = ret_32bit(simu, t, simu->registers[t][rs] + imm); */
    /* union { float f; int i; } f_and_i; */
    /* f_and_i.i = num; */
    /* simu->registers_f[t][ft] = f_and_i.f; */
    int32_t num;
    uint32_t from = simu->registers[t][rs] + imm;
    if (simu->mode == Parallel) {
        Spec *s;
        for (s = simu->spec_list[t]; s != NULL && s->addr != from; s = s->next);
        if (s == NULL) {
            num = ret_32bit(simu, t, from);
            union { float f; int i; } f_and_i;
            f_and_i.i = num;
            simu->registers_f[t][ft] = f_and_i.f;
            return;
        } else {
            num = s->data;
            union { float f; int i; } f_and_i;
            f_and_i.i = num;
            simu->registers_f[t][ft] = f_and_i.f;
            return;
        }
    } else {
        num = ret_32bit(simu, t, from);
        union { float f; int i; } f_and_i;
        f_and_i.i = num;
        simu->registers_f[t][ft] = f_and_i.f;
        return;
    }
    /* simu->pc[t] += 4; */
}

void sw_s(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    int32_t imm = get_imm(simu, t, unit);
    union { float f; int i; } f_and_i;
    f_and_i.f = simu->registers_f[t][ft];
    divide_8bits_store(simu, t, simu->registers[t][rs] + imm, f_and_i.i, simu->mode);
    /* simu->pc[t] += 4; */
}

void ftoi(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rs(simu, t, unit);
    uint32_t rt = get_rt(simu, t, unit);
//    union { float f; int i; } f_and_i;
//    f_and_i.f = simu->registers_f[fs];
//    simu->registers[rt] = f_and_i.i;
    int i_num;
    float f_num = simu->registers_f[t][fs];
    if(f_num >= 0) {
        i_num = (int)(f_num + 0.5);
    } else {
        i_num = (int)(f_num - 0.5);
    }
    simu->registers[t][rt] = i_num;
    /* simu->pc[t] += 4; */
}

void itof(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
//    union { float f; int i; } f_and_i;
//    f_and_i.i = simu->registers[rs];
//    simu->registers_f[ft] = f_and_i.f;
    simu->registers_f[t][ft] = (float)simu->registers[t][rs];
    /* simu->pc[t] += 4; */
}

void cadd(Simulator* simu, int t, int unit) {
    uint32_t rs = get_rs(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
//    union { float f; int i; } f_and_i;
//    f_and_i.i = simu->registers[rs];
//    simu->registers_f[ft] = f_and_i.f;
    int32_t num = ret_32bit(simu, t, simu->registers[t][rs]);
    union { float f; int i; } f_and_i;
    f_and_i.i = num;
    f_and_i.f += simu->registers_f[t][ft];
    divide_8bits_store(simu, t, simu->registers[t][rs], f_and_i.i, Normal);
    /* simu->pc[t] += 4; */
}

void c_eq_s(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rd(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);
    uint32_t cc = (fd >> 5) & 0b111;
    if (simu->registers_f[t][fs] == simu->registers_f[t][ft]) {
        simu->condition_code[t][cc] = 1;
    } else {
        simu->condition_code[t][cc] = 0;
    }
    /* simu->pc[t] += 4; */
}

void c_lt_s(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rd(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);
    uint32_t cc = (fd >> 5) & 0b111;
    if (simu->registers_f[t][fs] < simu->registers_f[t][ft]) {
        simu->condition_code[t][cc] = 1;
    } else {
        simu->condition_code[t][cc] = 0;
    }
    /* simu->pc[t] += 4; */
}

void c_le_s(Simulator* simu, int t, int unit) {
    uint32_t fs = get_rd(simu, t, unit);
    uint32_t ft = get_rt(simu, t, unit);
    uint32_t fd = ((get_imm(simu, t, unit) >> 3) & 0xff);
    uint32_t cc = (fd >> 5) & 0b111;
    if (simu->registers_f[t][fs] <= simu->registers_f[t][ft]) {
        simu->condition_code[t][cc] = 1;
    } else {
        simu->condition_code[t][cc] = 0;
    }
    /* simu->pc[t] += 4; */
}

void fork(Simulator* simu, int t, int unit) {
    simu->gc = simu->registers[t][get_rs(simu, t, unit)];
    simu->gd = get_imm(simu, t, unit);
    simu->mode = Parallel;

    if (t == 0) {
        for (int i=1; i<THREAD_NUM; i++) {
            for (int j=0; j<REGISTER_NUM; j++) {
                simu->registers[i][j] = simu->registers[0][j];
                simu->registers_f[i][j] = simu->registers_f[0][j];
            }
            for (int j=0; j<STACK_SIZE; j++) {
                simu->stack_field[i][j] = simu->stack_field[0][j];
            }
            simu->pc[i] = simu->pc[0];
        }
    }
    simu->pc[t] += 4;
}

void next(Simulator* simu, int t, int unit) {
    simu->registers[t][get_rt(simu, t, unit)] = simu->gc;
    simu->gc += simu->gd;
    simu->pc[t] += 4;
}

void end(Simulator* simu, int t, int unit) {
    if (t == 0) {
        for (int i=1; i<THREAD_NUM; i++) if (simu->pc[0] != simu->pc[i]) return;
        simu->mode = Normal;
        simu->pc[0] += 4;
    }
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

    set_inst(0b111011, dummybit, dmfmt, 1, &nop);
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
    set_inst(0b000000, 0b001001, dmfmt, 0, &jalr);
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
    set_inst(0b010001, 0b110010, 0b000, 3, &c_eq_s);
    set_inst(0b010001, 0b111100, 0b000, 3, &c_lt_s);
    set_inst(0b010001, 0b111110, 0b000, 3, &c_le_s);
    set_inst(0b110001, dummybit, dmfmt, 1, &lw_s);
    set_inst(0b111001, dummybit, dmfmt, 1, &sw_s);
    set_inst(0b111000, dummybit, dmfmt, 1, &ftoi);
    set_inst(0b110000, dummybit, dmfmt, 1, &itof);
    set_inst(0b111100, dummybit, dmfmt, 1, &cadd);
    set_inst(0b111101, dummybit, dmfmt, 1, &fork);
    set_inst(0b111110, dummybit, dmfmt, 1, &next);
    set_inst(0b111111, dummybit, dmfmt, 1, &end);


//    if (!file.open(QIODevice::WriteOnly)) {
//        qDebug() << "cannot open output.txt";
//        return;
//    }

    if((fp_out = fopen("output.ppm", "w")) == NULL) {
        printf("error");
        exit(1);
    }
}
