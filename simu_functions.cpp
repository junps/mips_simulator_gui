#include "simu_functions.h"

#include "math.h"

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

union Single bare_div(union Single a){
    union Single r;
    r.i =  (a.i & 0x007fffff);
    return r;
}//仮数部

union Single init_div(union Single a){//初期値
    union Single r, next;
    next.i = a.i + 1;
    r.f = (1 / (a.f * powf(2, 13)) + 1 / (next.f * powf(2, 13))) / 2;
    return r;
}

union Single constant_div(union Single x0, union Single a){//定数項(24bit数, 先頭は1)
    union Single r;
    r.i = 2 * x0.i - (int)(((long long int) x0.i * (long long int) x0.i * (long long int) a.i) >> 34);
    if ((a.i & 0x3ff) == 0x2 ||
        (a.i & 0x3ff) == 0x3 ||
        (a.i & 0x3ff) == 0x6 ||
        (a.i & 0x3ff) == 0x7 ||
        (a.i & 0x3ff) == 0x9 ||
        (a.i & 0x3ff) == 0xa ||
        (a.i & 0x3ff) == 0xe ||
        (a.i & 0x3ff) == 0x19 ||
        (a.i & 0x3ff) == 0x20||
        (a.i & 0x3ff) == 0x22)
        r.i = r.i + 1;//微調整
    return bare_div(r);
}

union Single grad_div(union Single x0){//勾配(13bit数)
    union Single r;
    r.i = (int) (((long long int) x0.i *(long long int) x0.i) >> 35);
    return bare_div(r);
}

union Single appr_div(union Single c, union Single g, union Single a){//近似値
    union Single r;
    r.i = c.i - (int)(((long int)a.i * (long int)g.i)>>12);
    return r;
}

union Single man_finv(union Single a){//aの逆数の仮数部計算
    union Single c, g, x0, a_l, a_m;
    a_l.i = (a.i & 0x1fff);//仮数下位
    a_m.i = ((a.i & 0x7fe000) >> 13) | 0x400;//仮数上位+ケチ表現
    x0.i = bare_div(init_div(a_m)).i | 0x800000;//初期値
    c = constant_div(x0, a_m);
    g = grad_div(x0);
    return appr_div(c, g, a_l);
}

union Single fdiv(union Single a, union Single b){//(a/b)
    union Single man_bi, man_a, m, r;
    int s = (a.i >> 31) ^ (b.i >> 31);
    int e_a = (a.i >> 23) & 0xff;
    int e_b = (b.i >> 23) & 0xff;
    int e = e_a + 126 - e_b;
    int u1 = e <= 0 ? 1 : 0;//underflow_flag1
    int u2 = e + 1 <= 0 ? 1 : 0;//underflow_flag2
    int ovf = e >= 255 ? 1 : 0;//overflow_flag
    man_bi.i = man_finv(b).i | 0x800000;
    man_a.i = (a.i & 0x7fffff) | 0x800000;
    m.i = (int)(((long long int)man_a.i * (long long int)man_bi.i)>>23);
    int man47 = m.i >> 24;
    if (ovf == 1){
        m.i = 0;
        e = 0xff;
    }else{
        if (man47 == 1){
            m.i = m.i>>1;
            e = e + 1;
            if (u2 == 1){
                e = 0;
                m.i = 0;
            }
        }else{
            if (u1 == 1){
                e = 0;
                m.i = 0;
            }
        }
    }
    r.i = (s<<31) | (e<<23) | (m.i & 0x7fffff);
    return r;
}

union Single init_sqrt(union Single k){//初期値
    union Single r, next, a;
    if ((k.i & 0x200)>>9 == 1){
        a.i = k.i;
        next.i = a.i + 1;
        r.f = (sqrtf(a.f * powf(2, 14)) + sqrtf(next.f * powf(2, 14))) / 2;
    } else {
        a.i = k.i | 0x200;
        next.i = a.i + 1;
        r.f = (sqrtf(a.f * powf(2, 15)) + sqrtf(next.f * powf(2, 15))) / 2;
    }
    return r;
}

union Single constant_sqrt(union Single x0, union Single k){//定数(24bit数, 先頭は1)
    union Single r, a;
    if ((k.i & 0x200)>>9 == 1){
        a.i = k.i;
        r.i = x0.i / 2 + ((long long int)a.i << 36) / x0.i;
    } else {
        a.i = k.i | 0x200;
        r.i = x0.i / 2 + ((long long int)a.i << 37) / x0.i;
    }
    return bare_div(r);
}

union Single grad_sqrt(union Single x0){//勾配(13bit数)
    union Single r;
    r.i = (int)((long long int) 0x1000000000 / (long long int)x0.i);
    return bare_div(r);
}

union Single appr_sqrt(union Single c, union Single g, union Single a,
                  int n){//近似値
    union Single r;
    if (n == 1) {
        r.i = c.i + (int)((long long int)g.i * (long long int)a.i >> 14);
    } else {
        r.i = c.i + (int)((long long int)g.i * (long long int)a.i >> 13);
    }
    return r;
}

union Single man_fsqrt(union Single a){//fsqrt(a)の仮数部
    union Single a_m, a_l, key, x0, c, g, r;
    int a_e = (a.i & 0x00800000) >> 23;
    a_l.i = (a.i & 0x3fff);
    a_m.i = ((a.i & 0x7fc000) >> 14);
    key.i = a_m.i | (a_e << 9);
    x0.i = 0x00800000 | bare_div(init_sqrt(key)).i;
    c = constant_sqrt(x0, key);
    g = grad_sqrt(x0);
    r = appr_sqrt(c, g, a_l, a_e);
    return r;
}

union Single fsqrt(union Single a){//fsqrt(a)
    union Single r;
    int e;
    if (((a.i >> 31) & 1) == 1){
        r.i = 0;
        return r;
    }
    int a_e = (a.i & 0x7f800000) >> 23;
    union Single m;
    m = man_fsqrt(a);
    if ((a_e & 1) == 1) {
        e = (a_e + 127) / 2;
    } else {
        e = (a_e + 126) / 2;
    }
    r.i = (e << 23) | m.i;
    return r;
}
