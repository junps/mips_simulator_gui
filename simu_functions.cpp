#include "simu_functions.h"

#include "math.h"

uint32_t ret_32bit(Simulator* simu, int thread, uint32_t from) {
    /* uint8_t eight_bit0 = simu->stack_field[from]; */
    /* uint8_t eight_bit1 = simu->stack_field[from + 1]; */
    /* uint8_t eight_bit2 = simu->stack_field[from + 2]; */
    /* uint8_t eight_bit3 = simu->stack_field[from + 3]; */
    uint8_t eight_bit0;
    uint8_t eight_bit1;
    uint8_t eight_bit2;
    uint8_t eight_bit3;
    if (from >= DATA_SIZE) {
        eight_bit0 = simu->stack_field[thread][from - DATA_SIZE];
        eight_bit1 = simu->stack_field[thread][from - DATA_SIZE + 1];
        eight_bit2 = simu->stack_field[thread][from - DATA_SIZE + 2];
        eight_bit3 = simu->stack_field[thread][from - DATA_SIZE + 3];
    } else {
        eight_bit0 = simu->data_field[from];
        eight_bit1 = simu->data_field[from + 1];
        eight_bit2 = simu->data_field[from + 2];
        eight_bit3 = simu->data_field[from + 3];
    }
    uint32_t inst = (uint32_t)((eight_bit0 << 24) | (eight_bit1 << 16) | (eight_bit2 << 8) | (eight_bit3));
    return inst;
}

void divide_8bits_store(Simulator* simu, int thread, uint32_t from, uint32_t num, Mode m) {
    if (from >= DATA_SIZE) {
        simu->stack_field[thread][from - DATA_SIZE] = (uint8_t)(num >> 24);
        simu->stack_field[thread][from - DATA_SIZE + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->stack_field[thread][from - DATA_SIZE + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->stack_field[thread][from - DATA_SIZE + 3] = (uint8_t)(num & (0b11111111));
        return;
    } else {
        if (thread == 0) {
            simu->data_field[from] = (uint8_t)(num >> 24);
            simu->data_field[from + 1] = (uint8_t)((num >> 16) & (0b11111111));
            simu->data_field[from + 2] = (uint8_t)((num >> 8) & (0b11111111));
            simu->data_field[from + 3] = (uint8_t)(num & (0b11111111));
        }
        if (m == Parallel) {
            if (simu->spec_list[thread] == NULL) {
                simu->spec_list[thread] = (Spec *)malloc(sizeof(Spec));
                simu->spec_list[thread]->addr = from;
                simu->spec_list[thread]->data = num;
                simu->spec_list[thread]->next = NULL;
                return;
            }
            Spec *s;
            for (s = simu->spec_list[thread]; s->next != NULL && s->addr != from; s = s->next);
            if (s->next == NULL) {
                Spec *new_spec = (Spec *)malloc(sizeof(Spec));
                new_spec->addr = from;
                new_spec->data = num;
                new_spec->next = NULL;
                s->next = new_spec;
                return;
            } else {
                s->data = num; return;
            }
        }
    }
}

uint64_t ret_inst_64bit(Simulator* simu, int thread, int unit) {
    uint8_t eight_bit0;
    uint8_t eight_bit1;
    uint8_t eight_bit2;
    uint8_t eight_bit3;
    uint8_t eight_bit4;
    uint8_t eight_bit5;
    uint64_t inst = 0b0;
    switch (unit) {
        case 0:
            eight_bit0 = simu->text_field0[simu->pc[thread]];
            eight_bit1 = simu->text_field0[simu->pc[thread] + 1];
            eight_bit2 = simu->text_field0[simu->pc[thread] + 2];
            eight_bit3 = simu->text_field0[simu->pc[thread] + 3];
            eight_bit4 = simu->text_field1[simu->pc[thread] + 0];
            eight_bit5 = simu->text_field1[simu->pc[thread] + 1];
            inst =
                ((1ul << 42) -1) &
                    (((uint64_t)eight_bit0 << 33) |
                     ((uint64_t)eight_bit1 << 25) |
                     ((uint64_t)eight_bit2 << 17) |
                     ((uint64_t)eight_bit3 << 9) |
                     ((uint64_t)eight_bit4 << 1) |
                     ((uint64_t)eight_bit5 >> 7));
            break;
        case 1:
            eight_bit0 = simu->text_field1[simu->pc[thread] + 1];
            eight_bit1 = simu->text_field1[simu->pc[thread] + 2];
            eight_bit2 = simu->text_field1[simu->pc[thread] + 3];
            eight_bit3 = simu->text_field2[simu->pc[thread] + 0];
            eight_bit4 = simu->text_field2[simu->pc[thread] + 1];
            eight_bit5 = simu->text_field2[simu->pc[thread] + 2];
            inst =
                ((1ul << 42) - 1) &
                    ((((uint64_t)(eight_bit0 & 0b1111111) << 34)) |
                     ((uint64_t)eight_bit1 << 26) |
                     ((uint64_t)eight_bit2 << 18) |
                     ((uint64_t)eight_bit3 << 10) |
                     ((uint64_t)eight_bit4 << 2) |
                     ((uint64_t)eight_bit5 >> 6));
            break;
        case 2:
            eight_bit0 = simu->text_field2[simu->pc[thread] + 2];
            eight_bit1 = simu->text_field2[simu->pc[thread] + 3];
            eight_bit2 = simu->text_field3[simu->pc[thread] + 0];
            eight_bit3 = simu->text_field3[simu->pc[thread] + 1];
            eight_bit4 = simu->text_field3[simu->pc[thread] + 2];
            eight_bit5 = simu->text_field3[simu->pc[thread] + 3];
            inst =
                ((1ul << 42) - 1) &
                    ((((uint64_t)(eight_bit0 & 0b111111) << 35)) |
                     ((uint64_t)eight_bit1 << 27) |
                     ((uint64_t)eight_bit2 << 19) |
                     ((uint64_t)eight_bit3 << 11) |
                     ((uint64_t)eight_bit4 << 3) |
                     ((uint64_t)eight_bit5 >> 5));
            break;
        default:
            perror("undefined unit");
            exit(1);
    }
    return inst;
}

uint32_t get_template(Simulator* simu, int t) {
    return simu->text_field3[simu->pc[t] + 3] & 0x1f;
}

uint32_t get_opcode(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return ((inst >> 35) & (0x3f));
}

uint32_t get_rs(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return ((inst >> 27) & (0xff));
}

uint32_t get_rt(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return ((inst >> 19) & (0xff));
}

uint32_t get_rd(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return ((inst >> 11) & (0xff));
}

uint32_t get_shift(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return ((inst >> 6) & (0x1f));
}

uint32_t get_func(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    if ((inst >> 34) == 0b0100011)
        // float case
        return inst & 0x7;
    else
        return inst & 0x3f;
}

int32_t get_imm(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    if(((inst) & (1 << 18))) {
        return (0x1fff << 19) | (inst & (0x7ffff));
    }
    return (inst & (0x7ffff));
}

uint32_t get_uimm(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return (inst & 0x7ffff);
}

uint32_t get_address(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return (uint32_t) (inst & 0xffffffff);
}

uint32_t get_fmt(Simulator* simu, int t, int unit) {
    uint64_t inst = ret_inst_64bit(simu, t, unit);
    return (((inst >> 33) & 3) << 1) | ((inst >> 22) & 1);
}

union Single bare(union Single a){
    union Single r;
    r.i =  (a.i & 0x007fffff);
    return r;
}//仮数部

union Single init(union Single a){//初期値
    union Single r, next;
    next.i = a.i + 1;
    r.f = (1 / (a.f * powf(2, 13)) + 1 / (next.f * powf(2, 13))) / 2;
    return r;
}

union Single constant(union Single x0, union Single a){//定数項(24bit数, 先頭は1)
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
    return bare(r);
}

union Single grad(union Single x0){//勾配(13bit数)
    union Single r;
    r.i = (int) (((long long int) x0.i *(long long int) x0.i) >> 35);
    return bare(r);
}

union Single appr(union Single c, union Single g, union Single a){//近似値
    union Single r;
    r.i = c.i - (int)(((long int)a.i * (long int)g.i)>>12);
    return r;
}

union Single man_finv(union Single a){//aの逆数の仮数部計算
    union Single c, g, x0, a_l, a_m;
    a_l.i = (a.i & 0x1fff);//仮数下位
    a_m.i = ((a.i & 0x7fe000) >> 13) | 0x400;//仮数上位+ケチ表現
    x0.i = bare(init(a_m)).i | 0x800000;//初期値
    c = constant(x0, a_m);
    g = grad(x0);
    return appr(c, g, a_l);
}

union Single pseudo_finv(union Single a){
    union Single m, r;
    int a_s = (a.i >> 31) & 1;
    int a_e = (a.i >> 23) & 0xff;
    m = man_finv(a);
    r.i = (a_s << 31) | (a_e << 23) | (m.i & 0x7fffff);
    return r;
}

union Single fmul_for_fdiv(union Single a, union Single b){
    union Single c;
    int a_s, a_e, a_m, b_s, b_e, b_m;
    a_s = (a.i >> 31) & 1;
    a_e = (a.i >> 23) & 0xff;
    a_m = (a.i & 0x7fffff) | 0x800000;//ケチ表現付き
    b_s = (b.i >> 31) & 1;
    b_e = (b.i >> 23) & 0xff;
    b_m = (b.i & 0x7fffff) | 0x800000;//ケチ表現付き
    int sy = a_s ^ b_s;
    int eyp = a_e + 126 - b_e;
    int eypi = eyp+1;
    int underflow1 = (eyp <= 0) ? 1 : 0;
    int underflow2 = (eypi <= 0) ? 1 : 0;
    int ovf_f = (eyp >= 255) ? 1 : 0;
    long long int my1 = (long long int)a_m * (long long int)b_m;
    int my48 = (my1 >> 47) & 1;
    int ovf = (ovf_f==1 || (my48 && (eypi >= 255))) ? 1 : 0;//ovf
    int ey =
    ovf ? 0xff :
    my48 && underflow2 ? 0 :
    my48 ? (eypi & 0xff) :
    underflow1 ? 0 :
    eyp & 0xff;
    int my =
    (ovf) ? 0 :
    my48 && underflow2 ? 0 :
    my48 ? (my1>>24) & 0x7fffff :
    underflow1 ? 0 :
    (my1>>23) & 0x7fffff;
    c.i = (sy<<31) | (ey<<23) | my;
    return c;
}

union Single fdiv(union Single a, union Single b){//(a/b)
    union Single b_inv, q;
    b_inv = pseudo_finv(b);
    q = fmul_for_fdiv(a, b_inv);
    return q;
}

union Single bare_sqrt(union Single a){
    union Single r;
    r.i =  (a.i & 0x007fffff);
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
    return bare_sqrt(r);
}

union Single grad_sqrt(union Single x0){//勾配(13bit数)
    union Single r;
    r.i = (int)((long long int) 0x1000000000 / (long long int)x0.i);
    return bare_sqrt(r);
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
    x0.i = 0x00800000 | bare_sqrt(init_sqrt(key)).i;
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

int LZC(int a){//LeadingZeroCounter
    int cnt = 0;
    for (int i = 0; i < 24; i++){
        if((a & 0x800000) == 0x800000){
            break;
        } else {
            cnt++;
            a = (a << 1) & 0xffffff;
        }
    }
    return cnt;
}


union Single fadd(union Single a, union Single b){//a + b
    union Single c;
    int a_s, a_e, a_m, b_s, b_e, b_m, l_s, l_e, l_m, s_s, s_e, s_m;
    a_s = (a.i >> 31) & 1;
    a_e = (a.i >> 23) & 0xff;
    a_m = (a.i & 0x7fffff) | 0x800000;
    b_s = (b.i >> 31) & 1;
    b_e = (b.i >> 23) & 0xff;
    b_m = (b.i & 0x7fffff) | 0x800000;
    if (a_e > b_e || (a_e == b_e && a_m > b_m)){
        l_s = a_s;
        l_e = a_e;
        l_m = a_m;
        s_s = b_s;
        s_e = b_e;
        s_m = b_m;
    } else {
        l_s = b_s;
        l_e = b_e;
        l_m = b_m;
        s_s = a_s;
        s_e = a_e;
        s_m = a_m;
    }
    int diff_e = l_e - s_e > 24 ? 24 : l_e - s_e;
    int s_m_shifted = s_m >> diff_e;
    int m_raw;
    if (s_s ^ l_s) {
        m_raw = l_m - s_m_shifted;
    } else {
        m_raw = l_m + s_m_shifted;
    }
    int m25 = (m_raw >> 24) & 1;
    int shift_m = LZC(m_raw);
    int m =
        m25==1 ? (m_raw >> m25) & 0x7fffff :
        (m_raw << shift_m) & 0x7fffff;
    int e =
    m25==1 ? l_e + m25 :
    l_e - shift_m;

    c.i = (e <= 0) ? (l_s << 31) :
    (e >= 255) ? (l_s << 31) | (0xff << 23) :
    (l_s << 31) | (e << 23) | m;
    return c;
}

union Single fsub(union Single a, union Single b){//a - b
    union Single c;
    int a_s, a_e, a_m, b_s, b_e, b_m, l_s, l_e, l_m, s_s, s_e, s_m;
    a_s = (a.i >> 31) & 1;
    a_e = (a.i >> 23) & 0xff;
    a_m = (a.i & 0x7fffff) | 0x800000;
    b_s = (b.i >> 31) & 1;
    b_e = (b.i >> 23) & 0xff;
    b_m = (b.i & 0x7fffff) | 0x800000;
    if (a_e > b_e || (a_e == b_e && a_m > b_m)){
        l_s = a_s;
        l_e = a_e;
        l_m = a_m;
        s_s = b_s;
        s_e = b_e;
        s_m = b_m;
    } else {
        l_s = b_s;
        l_e = b_e;
        l_m = b_m;
        s_s = a_s;
        s_e = a_e;
        s_m = a_m;
    }
    int diff_e = l_e - s_e > 24 ? 24 : l_e - s_e;
    int s_m_shifted = s_m >> diff_e;
    int m_raw;
    if (s_s ^ l_s) {
        m_raw = l_m + s_m_shifted;
    } else {
        m_raw = l_m - s_m_shifted;
    }
    int m25 = (m_raw >> 24) & 1;
    int shift_m = LZC(m_raw);
    int m =
        m25==1 ? (m_raw >> m25) & 0x7fffff :
        (m_raw << shift_m) & 0x7fffff;
    int e =
    m25==1 ? l_e + m25 :
    l_e - shift_m > 0 ? l_e - shift_m :
    0;
    int s;
    if (a_e > b_e || (a_e == b_e && a_m > b_m)){
        if (a_s == 0){
            s = 0;
        } else {
            s = 1;
        }
    } else {
        if (b_s == 1){
            s = 0;
        } else {
            s = 1;
        }
    }

    c.i = (e == 0) ? (s << 31) :
    (e >= 255) ? (s << 31) | (0xff << 23) :
    (s << 31) | (e << 23) | m;
    return c;
}

union Single fmul(union Single a, union Single b){//a * b
    union Single c;
    int a_s, a_e, a_m, b_s, b_e, b_m;
    a_s = (a.i >> 31) & 1;
    a_e = (a.i >> 23) & 0xff;
    a_m = (a.i & 0x7fffff) | 0x800000;//ケチ表現付き
    b_s = (b.i >> 31) & 1;
    b_e = (b.i >> 23) & 0xff;
    b_m = (b.i & 0x7fffff) | 0x800000;//ケチ表現付き
    int sy = a_s ^ b_s;
    int eyp = a_e + b_e - 127;
    int eypi = eyp+1;
    int underflow = (eyp < 0 || a_e == 0 || b_e == 0) ? 1 : 0;
    int ovf_f = (eyp >= 255) ? 1 : 0;
    long long int my1 = (long long int)a_m * (long long int)b_m;
    int my48 = (my1 >> 47) & 1;
    int ovf = (ovf_f==1 || (my48 && (eypi >= 255))) ? 1 : 0;//ovf
    int ey =
    underflow ? 0 :
    ovf ? 255 :
    my48 ? (eypi & 0xff) : eyp & 0xff;
    int my =
    (underflow || ovf) ? 0 :
    my48 ? (my1>>24) & 0x7fffff : (my1>>23) & 0x7fffff;
    c.i = (sy<<31) | (ey<<23) | my;
    return c;
}
