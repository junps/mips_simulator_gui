#include "no_gui.h"

#include <QDebug>

Simulator* create_simu_(uint32_t pc, uint32_t sp) {
    Simulator* simu = (Simulator*)malloc(sizeof(Simulator));
    simu->text_field = (uint8_t*)malloc(TEXT_SIZE);
    simu->stack_field = (uint8_t*)malloc(STACK_SIZE);
    simu->data_field = (uint8_t*)malloc(DATA_SIZE);

    memset(simu->registers, 0, sizeof(simu->registers));
    memset(simu->registers_f, 0, sizeof(simu->registers));
    memset(simu->condition_code, 0, sizeof(simu->condition_code));
    memset(simu->text_field, 0, TEXT_SIZE);
    memset(simu->data_field, 0, DATA_SIZE);
    memset(simu->stack_field, 0, STACK_SIZE);

    simu->pc = pc;
    simu->registers[SP] = sp;
    simu->registers[ZERO] = 0;

    return simu;
}

void exec_all(QString file_name, char* sld_file) {
    string file_name_std = file_name.toStdString();

    int n = file_name_std.size();
    string base_name = file_name_std.substr(0, n - 9);
    string data_name = base_name + "data.mem";
    string debug_name = base_name + "debug.txt";

    QString data_file = QString::fromStdString(data_name);
    QString debug_file = QString::fromStdString(debug_name);

    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "cannot open the file";
    }
    QTextStream in(&file);
    QString input_text = in.readLine();
    int cnt = 0;
    char *buf;
    buf = (char *)calloc(1024, sizeof(char));

    Simulator* simu;
    remove("output.ppm");

    load_sld_file(sld_file);

    init_instructions();

    simu = create_simu_(0, 0);

    while(!input_text.isNull()) {
        buf = input_text.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf, NULL, 2);
        simu->text_field[cnt] = (uint8_t)(num >> 24);
        simu->text_field[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->text_field[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->text_field[cnt + 3] = (uint8_t)(num & (0b11111111));
        input_text = in.readLine(256);
        cnt += 4;
    }
    file.close();

    QFile file_d(data_file);
    if(!file_d.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "cannot open the file";
    }
    QTextStream in_d(&file_d);
    QString input_text_d = in_d.readLine();
    int cnt_d = 0;
    char *buf_d;
    buf_d = (char *)calloc(1024, sizeof(char));

    while(!input_text_d.isNull()) {
        buf_d = input_text_d.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf_d, NULL, 2);
        simu->data_field[cnt_d] = (uint8_t)(num >> 24);
        simu->data_field[cnt_d + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->data_field[cnt_d + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->data_field[cnt_d + 3] = (uint8_t)(num & (0b11111111));
        input_text_d = in_d.readLine(256);
        cnt_d += 4;
    }
    file_d.close();

    uint32_t pre_pc = -1;
    while(1) {
        uint32_t opcode = get_opcode(simu);
        uint32_t funct = get_func(simu);
        uint32_t fmt = get_fmt(simu);

//        printf("opcode : %d, funct : %d\n", opcode, funct);
        if(pre_pc == simu->pc) break;
        pre_pc = simu->pc;

        /* if(opcode == 0b111111) break; */

        if (instructions[opcode][funct][fmt] == NULL) {
            printf("\n\nNot Implemented: opcode : %x, funct : %x\n", opcode, funct);
            printf("pc is %d\n", simu->pc / 4);
            exit(1);
        }

        instructions[opcode][funct][fmt](simu);
    }
}
