#ifndef SIMU_FUNCTIONS_H
#define SIMU_FUNCTIONS_H

#include <stdint.h>

#include "simulator.h"

uint32_t ret_32bit(Simulator* simu, int t, uint32_t from);

void divide_8bits_store(Simulator* simu, int t, uint32_t from, uint32_t num, Mode m);

uint64_t ret_inst_64bit(Simulator* simu, int t, int unit);

uint32_t get_template(Simulator* simu, int t);

uint32_t get_opcode(Simulator* simu, int t, int unit);

uint32_t get_rs(Simulator* simu, int t, int unit);

uint32_t get_rt(Simulator* simu, int t, int unit);

uint32_t get_rd(Simulator* simu, int t, int unit);

uint32_t get_shift(Simulator* simu, int t, int unit);

uint32_t get_func(Simulator* simu, int t, int unit);

int32_t get_imm(Simulator* simu, int t, int unit);

uint32_t get_uimm(Simulator* simu, int t, int unit);

uint32_t get_address(Simulator* simu, int t, int unit);

uint32_t get_fmt(Simulator* simu, int t, int unit);

union Single {
    float f;
    int i;
};

union Single fdiv(union Single a, union Single b);

union Single fsqrt(union Single a);

union Single fadd(union Single a, union Single b);

union Single fsub(union Single a, union Single b);

union Single fmul(union Single a, union Single b);

#endif // SIMU_FUNCTIONS_H
