#include "simu_functions.h"

uint32_t ret_32bit(Simulator* simu, uint32_t from) {
    uint8_t eight_bit0 = simu->stack_field[from];
    uint8_t eight_bit1 = simu->stack_field[from + 1];
    uint8_t eight_bit2 = simu->stack_field[from + 2];
    uint8_t eight_bit3 = simu->stack_field[from + 3];
    uint32_t inst = (uint32_t)((eight_bit0 << 24) | (eight_bit1 << 16) | (eight_bit2 << 8) | (eight_bit3));
    return inst;
}

void divide_8bits_store(Simulator* simu, uint32_t from, uint32_t num) {
    simu->stack_field[from] = (uint8_t)(num >> 24);
    simu->stack_field[from + 1] = (uint8_t)((num >> 16) & (0b11111111));
    simu->stack_field[from + 2] = (uint8_t)((num >> 8) & (0b11111111));
    simu->stack_field[from + 3] = (uint8_t)(num & (0b11111111));
}

uint32_t ret_inst_32bit(Simulator* simu) {
    uint8_t eight_bit0 = simu->text_field[simu->pc];
    uint8_t eight_bit1 = simu->text_field[simu->pc + 1];
    uint8_t eight_bit2 = simu->text_field[simu->pc + 2];
    uint8_t eight_bit3 = simu->text_field[simu->pc + 3];
    uint32_t inst = (uint32_t)((eight_bit0 << 24) | (eight_bit1 << 16) | (eight_bit2 << 8) | (eight_bit3));
    return inst;
}

uint32_t get_opcode(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return ((inst >> 26) & (0x3f));
}

uint32_t get_rs(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return ((inst >> 21) & (0x1f));
}

uint32_t get_rt(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return ((inst >> 16) & (0x1f));
}

uint32_t get_rd(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return ((inst >> 11) & (0x1f));
}

uint32_t get_shift(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return ((inst >> 6) & (0x1f));
}

uint32_t get_func(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return ((inst) & (0x3f));
}

int32_t get_imm(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    if(((inst) & (1 << 15))) {
        return (0xffff << 16) | (inst & (0xffff));
    }
    return (inst & (0xffff));
}

uint32_t get_address(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return ((inst) & (0x3ffffff));
}

uint32_t get_fmt(Simulator* simu) {
    uint32_t inst = ret_inst_32bit(simu);
    return (((inst >> 24) & 3) << 1) | ((inst >> 16) & 1);
}
