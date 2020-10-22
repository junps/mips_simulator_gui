#ifndef SIMU_FUNCTIONS_H
#define SIMU_FUNCTIONS_H

#include <stdint.h>

#include "simulator.h"

uint32_t ret_32bit(Simulator* simu, uint32_t from);

void divide_8bits_store(Simulator* simu, uint32_t from, uint32_t num);

uint32_t ret_inst_32bit(Simulator* simu);

uint32_t get_opcode(Simulator* simu);

uint32_t get_rs(Simulator* simu);

uint32_t get_rt(Simulator* simu);

uint32_t get_rd(Simulator* simu);

uint32_t get_shift(Simulator* simu);

uint32_t get_func(Simulator* simu);

int32_t get_imm(Simulator* simu);

uint32_t get_address(Simulator* simu);

#endif // SIMU_FUNCTIONS_H
