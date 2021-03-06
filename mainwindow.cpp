﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QListWidgetItem>
#include <Qt>

using namespace std;

#include "simu_functions.h"
#include "instruction.h"

#include <string.h>

//extern FILE *fp_out;

void MainWindow::open_inst_file(QString inst_file) {
    file_name = inst_file;

    string file_name_std = file_name.toStdString();

    string base_name = file_name_std;
    string text_name0 = base_name + "_instr0.mem";
    string text_name1 = base_name + "_instr1.mem";
    string text_name2 = base_name + "_instr2.mem";
    string text_name3 = base_name + "_instr3.mem";
    string data_name = base_name + "_data.mem";
    string debug_name = base_name + "_debug.txt";

    QString text_file0 = QString::fromStdString(text_name0);
    QString text_file1 = QString::fromStdString(text_name1);
    QString text_file2 = QString::fromStdString(text_name2);
    QString text_file3 = QString::fromStdString(text_name3);
    QString data_file = QString::fromStdString(data_name);
    QString debug_file = QString::fromStdString(debug_name);

    displayFile(debug_file);

    QFile file0(text_file0);
    if(!file0.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
    }
    QTextStream in0(&file0);
    QString input_text = in0.readLine();
    int cnt = 0;
    char *buf;
    buf = (char *)calloc(1024, sizeof(char));
    while(!input_text.isNull()) {
        buf = input_text.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf, NULL, 2);
        simu->text_field0[cnt] = (uint8_t)(num >> 24);
        simu->text_field0[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->text_field0[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->text_field0[cnt + 3] = (uint8_t)(num & (0b11111111));
        input_text = in0.readLine(256);
        cnt += 4;
    }
    file0.close();
    QFile file1(text_file1);
    if(!file1.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
    }
    QTextStream in1(&file1);
    input_text = in1.readLine();
    cnt = 0;
    buf = (char *)calloc(1024, sizeof(char));
    while(!input_text.isNull()) {
        buf = input_text.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf, NULL, 2);
        simu->text_field1[cnt] = (uint8_t)(num >> 24);
        simu->text_field1[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->text_field1[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->text_field1[cnt + 3] = (uint8_t)(num & (0b11111111));
        input_text = in1.readLine(256);
        cnt += 4;
    }
    file1.close();
    QFile file2(text_file2);
    if(!file2.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
    }
    QTextStream in2(&file2);
    input_text = in2.readLine();
    cnt = 0;
    buf = (char *)calloc(1024, sizeof(char));
    while(!input_text.isNull()) {
        buf = input_text.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf, NULL, 2);
        simu->text_field2[cnt] = (uint8_t)(num >> 24);
        simu->text_field2[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->text_field2[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->text_field2[cnt + 3] = (uint8_t)(num & (0b11111111));
        input_text = in2.readLine(256);
        cnt += 4;
    }
    file2.close();
    QFile file3(text_file3);
    if(!file3.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
    }
    QTextStream in3(&file3);
    input_text = in3.readLine();
    cnt = 0;
    buf = (char *)calloc(1024, sizeof(char));
    while(!input_text.isNull()) {
        buf = input_text.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf, NULL, 2);
        simu->text_field3[cnt] = (uint8_t)(num >> 24);
        simu->text_field3[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->text_field3[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->text_field3[cnt + 3] = (uint8_t)(num & (0b11111111));
        input_text = in3.readLine(256);
        cnt += 4;
    }
    file3.close();

    QFile file_d(data_file);
    if(!file_d.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
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
    display_last_stacks(simu);
    file_d.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitialTableDisplay();

    remove("output.ppm");

//    if((fp_out = fopen("output.txt", "w")) == NULL) {
//        printf("error");
//        exit(1);
//    }

    init_instructions();

    simu = create_simu(0, 0);

    for(int i = 0; i < 32 + 32 + 1 + 8; i++) {
        ui->tableWidget_Registers->setRowHeight(i, 27);
    }
//    for(int i = 0; i < l_lis.stack_size; i++) {
//        ui->tableWidget_Memory->setRowHeight(i, 27);
//    }

    QStringList arg_list = QApplication::arguments();
    QByteArray ba = arg_list[1].toLocal8Bit();
    const char* sld_file = ba.data();
    load_sld_file(sld_file);

    if(arg_list.size() == 4) {
        open_inst_file(arg_list[2]);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::InitialTableDisplay()
{
    ui->tableWidget_Registers->setRowCount(73);
    ui->tableWidget_Registers->setColumnCount(3);

    QStringList vlabels;
    vlabels << "ZERO" << "AT" << "V0" << "V1" << "A0" << "A1" << "A2" << "A3" << "T0" << "T1"
            << "T2" << "T3" << "T4" << "T5" << "T6" << "T7" << "S0" << "S1" << "S2" << "S3"
            << "S4" << "S5" << "S6" << "S7" << "T8" << "T9" << "K0" << "K1" << "GP" << "SP" << "FP" << "RA";

    vlabels << "F0" << "F1" << "F2" << "F3" << "F4" << "F5" << "F6" << "F7" << "F8" << "F9" << "F10" << "F11"
            << "F12" << "F13" << "F14" << "F15" << "F16" << "F17" << "F18" << "F19" << "F20" << "F21" << "F22"
            << "F23" << "F24" << "F25" << "F26" << "F27" << "F28" << "F29" << "F30" << "F31" << "N_pc";

    vlabels << "C0" << "C1" << "C2" << "C3" << "C4" << "C5" << "C6" << "C7";

    ui->tableWidget_Registers->setVerticalHeaderLabels(vlabels);

    QStringList hlabels;
    hlabels << "pre" << "now" << "next";
    ui->tableWidget_Registers->setHorizontalHeaderLabels(hlabels);

    for(int i = 0; i < 73; i++) {
        for (int j = 0; j < 3; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(0, 16));
            ui->tableWidget_Registers->setItem(i, j, item);
        }
    }

    ui->tableWidget_Registers->setAlternatingRowColors(true);

    ui->tableWidget_Memory->setRowCount(l_lis.data_size + 1);
    ui->tableWidget_Memory->setColumnCount(3);

//    QStringList vlabels2;
//    for(int i = 0; i <= l_lis.stack_size; i++) {
//        vlabels2 << QString::number(l_lis.ini_sp + i);
//    }

    //ui->tableWidget_Memory->setVerticalHeaderLabels(vlabels2);

    QStringList hlabels2;
    hlabels2 << "pre" << "now" << "next";
    ui->tableWidget_Memory->setHorizontalHeaderLabels(hlabels2);

    for(int i = 0; i <= l_lis.data_size; i++) {
        for (int j = 0; j < 3; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(0, 16));
            ui->tableWidget_Memory->setItem(i, j, item);
        }
    }

    //ui->tableWidget_Memory->setAlternatingRowColors(true);

}

void MainWindow::update_register_table()
{
    // now_node
    for(int i = 0; i < 32; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 8);
        sprintf(x, "%8x", l_lis.now_node->registers[0][i]);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(l_lis.now_node->registers[i], 16));
        ui->tableWidget_Registers->setItem(i, 1, item);
    }

    for(int j = 0; j < 32; j++) {
        union { float f; int i; } f_and_i;
        f_and_i.f = l_lis.now_node->registers_f[0][j];
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 8);
        sprintf(x, "%8x", f_and_i.i);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(f_and_i.i, 16));
        ui->tableWidget_Registers->setItem(j + 32, 1, item);
    }

    QTableWidgetItem *item1 = new QTableWidgetItem;
    char* x1;
    x1 = (char*)malloc(sizeof(char*) * 8);
    sprintf(x1, "%8x", l_lis.now_node->pc[0]);
    item1->setText(QString(x1));
    free(x1);
    //item1->setText(QString::number(l_lis.now_node->pc, 16));
    ui->tableWidget_Registers->setItem(64, 1, item1);

    for(int i = 0; i < 8; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->condition_code[0][i], 16));
        ui->tableWidget_Registers->setItem(i + 65, 1, item);
    }

    // now_node->prev
    if(l_lis.now_node == l_lis.boss) {
        for(int i = 0; i < 32; i++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            char* x;
            x = (char*)malloc(sizeof(char*) * 8);
            sprintf(x, "%8x", l_lis.now_node->registers[0][i]);
            item->setText(QString(x));
            free(x);
            //item->setText(QString::number(l_lis.now_node->registers[i], 16));
            ui->tableWidget_Registers->setItem(i, 0, item);
        }

        for(int j = 0; j < 32; j++) {
            union { float f; int i; } f_and_i;
            f_and_i.f = l_lis.now_node->registers_f[0][j];
            QTableWidgetItem *item = new QTableWidgetItem;
            char* x;
            x = (char*)malloc(sizeof(char*) * 8);
            sprintf(x, "%8x", f_and_i.i);
            item->setText(QString(x));
            free(x);
            //item->setText(QString::number(f_and_i.i, 16));
            ui->tableWidget_Registers->setItem(j + 32, 0, item);
        }

        QTableWidgetItem *item0 = new QTableWidgetItem;
        char* x0;
        x0 = (char*)malloc(sizeof(char*) * 8);
        sprintf(x0, "%8x", l_lis.now_node->pc[0]);
        item0->setText(QString(x0));
        free(x0);
        //item0->setText(QString::number(l_lis.now_node->pc, 16));
        ui->tableWidget_Registers->setItem(64, 0, item0);

        for(int i = 0; i < 8; i++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(l_lis.now_node->condition_code[0][i], 16));
            ui->tableWidget_Registers->setItem(i + 65, 0, item);
        }
    } else {
        for(int i = 0; i < 32; i++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            char* x;
            x = (char*)malloc(sizeof(char*) * 8);
            sprintf(x, "%8x", l_lis.now_node->prev->registers[0][i]);
            item->setText(QString(x));
            free(x);
            //item->setText(QString::number(l_lis.now_node->prev->registers[i], 16));
            ui->tableWidget_Registers->setItem(i, 0, item);
        }

        for(int j = 0; j < 32; j++) {
            union { float f; int i; } f_and_i;
            f_and_i.f = l_lis.now_node->prev->registers_f[0][j];
            QTableWidgetItem *item = new QTableWidgetItem;
            char* x;
            x = (char*)malloc(sizeof(char*) * 8);
            sprintf(x, "%8x", f_and_i.i);
            item->setText(QString(x));
            free(x);
            //item->setText(QString::number(f_and_i.i, 16));
            ui->tableWidget_Registers->setItem(j + 32, 0, item);
        }

        QTableWidgetItem *item0 = new QTableWidgetItem;
        char* x0;
        x0 = (char*)malloc(sizeof(char*) * 8);
        sprintf(x0, "%8x", l_lis.now_node->prev->pc[0]);
        item0->setText(QString(x0));
        free(x0);
        //item0->setText(QString::number(l_lis.now_node->prev->pc, 16));
        ui->tableWidget_Registers->setItem(64, 0, item0);

        for(int i = 0; i < 8; i++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(l_lis.now_node->prev->condition_code[0][i], 16));
            ui->tableWidget_Registers->setItem(i + 65, 0, item);
        }
    }


    //now_node->next
    for(int i = 0; i < 32; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 8);
        sprintf(x, "%8x", l_lis.now_node->registers[0][i]);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(l_lis.now_node->next->registers[i], 16));
        ui->tableWidget_Registers->setItem(i, 2, item);
    }

    for(int j = 0; j < 32; j++) {
        union { float f; int i; } f_and_i;
        f_and_i.f = l_lis.now_node->next->registers_f[0][j];
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 8);
        sprintf(x, "%8x", f_and_i.i);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(f_and_i.i, 16));
        ui->tableWidget_Registers->setItem(j + 32, 2, item);
    }

    QTableWidgetItem *item2 = new QTableWidgetItem;
    char* x2;
    x2 = (char*)malloc(sizeof(char*) * 8);
    sprintf(x2, "%8x", l_lis.now_node->next->pc[0]);
    item2->setText(QString(x2));
    free(x2);
    //item2->setText(QString::number(l_lis.now_node->next->pc, 16));
    ui->tableWidget_Registers->setItem(64, 2, item2);

    for(int i = 0; i < 8; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->next->condition_code[0][i], 16));
        ui->tableWidget_Registers->setItem(i + 65, 2, item);
    }

    for(int i = 0; i < 32; i++) {
        if(l_lis.now_node->prev->registers[0][i] != l_lis.now_node->registers[0][i]) {
            ui->tableWidget_Registers->item(i, 0)->setBackground(Qt::red);
            ui->tableWidget_Registers->item(i, 1)->setBackground(Qt::green);
        }
    }

    for(int i = 0; i < 32; i++) {
        if(l_lis.now_node->prev->registers_f[0][i] != l_lis.now_node->registers_f[0][i]) {
            ui->tableWidget_Registers->item(i + 32, 0)->setBackground(Qt::red);
            ui->tableWidget_Registers->item(i + 32, 1)->setBackground(Qt::green);
        }
    }

    if(l_lis.now_node->prev->pc != l_lis.now_node->pc) {
        ui->tableWidget_Registers->item(64, 0)->setBackground(Qt::red);
        ui->tableWidget_Registers->item(64, 1)->setBackground(Qt::green);
    }

    for(int i = 0; i < 8; i++) {
        if(l_lis.now_node->prev->condition_code[0][i] != l_lis.now_node->condition_code[0][i]) {
            ui->tableWidget_Registers->item(i + 65, 0)->setBackground(Qt::red);
            ui->tableWidget_Registers->item(i + 65, 1)->setBackground(Qt::green);
        }
    }
}

void MainWindow::update_memory_table()
{
    // now_node
    for(int i = 0; i <= l_lis.data_size; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 2);
        sprintf(x, "%8x", l_lis.now_node->data[i]);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(l_lis.now_node->stack[i], 16));
        ui->tableWidget_Memory->setItem(i, 1, item);
    }

    // now_node->prev
    if(l_lis.now_node == l_lis.boss) {
        for(int i = 0; i <= l_lis.data_size; i++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            char* x;
            x = (char*)malloc(sizeof(char*) * 2);
            sprintf(x, "%8x", l_lis.now_node->data[i]);
            item->setText(QString(x));
            free(x);
            //item->setText(QString::number(l_lis.now_node->stack[i], 16));
            ui->tableWidget_Memory->setItem(i, 0, item);
        }
    } else {
        for(int i = 0; i <= l_lis.data_size; i++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            char* x;
            x = (char*)malloc(sizeof(char*) * 2);
            sprintf(x, "%8x", l_lis.now_node->prev->data[i]);
            item->setText(QString(x));
            free(x);
            //item->setText(QString::number(l_lis.now_node->prev->stack[i], 16));
            ui->tableWidget_Memory->setItem(i, 0, item);
        }
    }

    //now_node->next
    for(int i = 0; i <= l_lis.data_size; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 2);
        sprintf(x, "%8x", l_lis.now_node->next->data[i]);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(l_lis.now_node->next->stack[i], 16));
        ui->tableWidget_Memory->setItem(i, 2, item);
    }


    for(int i = 0; i <= l_lis.data_size; i++) {
        if(l_lis.now_node->prev->data[i] != l_lis.now_node->data[i]) {
            ui->tableWidget_Memory->setCurrentItem(ui->tableWidget_Memory->item(i, 1));
            ui->tableWidget_Memory->scrollToItem(ui->tableWidget_Memory->currentItem());

            ui->tableWidget_Memory->item(i, 0)->setBackground(Qt::red);
            ui->tableWidget_Memory->item(i, 1)->setBackground(Qt::green);
        }
    }
}

void MainWindow::displayFile(QString file_name)
{
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
    }
    QTextStream in(&file);
    QString input_text = in.readLine();
    int i = 0;
    while(!input_text.isNull()) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::number(i) + "    " + input_text);
        ui->listWidget_Code->addItem(item);
        input_text = in.readLine(256);
        i++;
    }
    file.close();
    heigh_light_row(0);
}

void MainWindow::heigh_light_row(int row)
{
    ui->listWidget_Code->setCurrentRow(row);
    ui->listWidget_Code->scrollToItem(ui->listWidget_Code->currentItem());
}

void MainWindow::back_simu(Small_simu *small_simu)
{
    for (int j=0; j<THREAD_NUM; j++) {
        simu->pc[j] = small_simu->pc[j];

        for (int i = 0; i < 32; i++) {
            simu->registers[j][i] = small_simu->registers[j][i];
        }

        for (int i = 0; i < 32; i++) {
            simu->registers_f[j][i] = small_simu->registers_f[j][i];
        }

        for (int i = 0; i < 8; i++) {
            simu->condition_code[j][i] = small_simu->condition_code[j][i];
        }

        for(int i = 0; i <= l_lis.stack_size; i++) {
            simu->stack_field[j][l_lis.ini_sp + i] = small_simu->stack[j][i];
        }
    }
}

void MainWindow::display_last_register(Simulator *simu)
{
    // now_node
//    for(int i = 0; i < 32; i++) {
//        QTableWidgetItem *item = new QTableWidgetItem;
//        item->setText(QString::number(simu->registers[i], 16));
//        ui->tableWidget_Registers->setItem(i, 1, item);
//    }

//    for(int j = 0; j < 32; j++) {
//        union { float f; int i; } f_and_i;
//        f_and_i.f = simu->registers_f[j];
//        QTableWidgetItem *item = new QTableWidgetItem;
//        item->setText(QString::number(f_and_i.i, 16));
//        ui->tableWidget_Registers->setItem(j + 32, 1, item);
//    }
//    QTableWidgetItem *item1 = new QTableWidgetItem;
//    item1->setText(QString::number(simu->pc, 16));
//    ui->tableWidget_Registers->setItem(64, 1, item1);

//    for(int i = 0; i < 8; i++) {
//        QTableWidgetItem *item = new QTableWidgetItem;
//        item->setText(QString::number(simu->condition_code[i], 16));
//        ui->tableWidget_Registers->setItem(i + 65, 1, item);
//    }

    // now_node
    for(int i = 0; i < 32; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 8);
        sprintf(x, "%8x", simu->registers[0][i]);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(l_lis.now_node->registers[i], 16));
        ui->tableWidget_Registers->setItem(i, 1, item);
    }

    for(int j = 0; j < 32; j++) {
        union { float f; int i; } f_and_i;
        f_and_i.f = simu->registers_f[0][j];
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 8);
        sprintf(x, "%8x", f_and_i.i);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(f_and_i.i, 16));
        ui->tableWidget_Registers->setItem(j + 32, 1, item);
    }

    QTableWidgetItem *item1 = new QTableWidgetItem;
    char* x1;
    x1 = (char*)malloc(sizeof(char*) * 8);
    sprintf(x1, "%8x", simu->pc[0]);
    item1->setText(QString(x1));
    free(x1);
    //item1->setText(QString::number(l_lis.now_node->pc, 16));
    ui->tableWidget_Registers->setItem(64, 1, item1);

    for(int i = 0; i < 8; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(simu->condition_code[0][i], 16));
        ui->tableWidget_Registers->setItem(i + 65, 1, item);
    }
}

void MainWindow::display_last_data(Simulator *simu)
{
    // now_node
    for(int i = 0; i <= l_lis.data_size; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 2);
        sprintf(x, "%8x", simu->data_field[i]);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(simu->stack_field[i], 16));
        ui->tableWidget_Memory->setItem(i, 1, item);
    }
}

void MainWindow::display_last_stacks(Simulator *simu)
{
    // now_node
    for(int i = 0; i <= l_lis.stack_size; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        char* x;
        x = (char*)malloc(sizeof(char*) * 2);
        sprintf(x, "%8x", simu->stack_field[0][i]);
        item->setText(QString(x));
        free(x);
        //item->setText(QString::number(simu->stack_field[i], 16));
        ui->tableWidget_Memory->setItem(i, 1, item);
    }
}


void MainWindow::on_pushButton_Open_released()
{
    /* file_name = QFileDialog::getOpenFileName(this, "open a file",""); */

    /* string file_name_std = file_name.toStdString(); */

    /* string base_name = file_name_std; */
    /* string text_name0 = base_name + "_instr0.mem"; */
    /* string text_name1 = base_name + "_instr1.mem"; */
    /* string text_name2 = base_name + "_instr2.mem"; */
    /* string text_name3 = base_name + "_instr3.mem"; */
    /* string data_name = base_name + "_data.mem"; */
    /* string debug_name = base_name + "_debug.txt"; */

    /* QString text_file0 = QString::fromStdString(text_name0); */
    /* QString text_file1 = QString::fromStdString(text_name1); */
    /* QString text_file2 = QString::fromStdString(text_name2); */
    /* QString text_file2 = QString::fromStdString(text_name2); */
    /* QString data_file = QString::fromStdString(data_name); */
    /* QString debug_file = QString::fromStdString(debug_name); */

    /* displayFile(debug_file); */

    /* QFile file(text_file0); */
    /* if(!file0.open(QFile::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this,"title","cannot open the file"); */
    /* } */
    /* QTextStream in(&file0); */
    /* QString input_text = in.readLine(); */
    /* int cnt = 0; */
    /* char *buf; */
    /* buf = (char *)calloc(1024, sizeof(char)); */
    /* while(!input_text.isNull()) { */
    /*     buf = input_text.toUtf8().data(); */
    /*     uint32_t num = (uint32_t)strtol(buf, NULL, 2); */
    /*     simu->text_field0[cnt] = (uint8_t)(num >> 24); */
    /*     simu->text_field0[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111)); */
    /*     simu->text_field0[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111)); */
    /*     simu->text_field0[cnt + 3] = (uint8_t)(num & (0b11111111)); */
    /*     input_text = in.readLine(256); */
    /*     cnt += 4; */
    /* } */
    /* file0.close(); */
    /* QFile file(text_file1); */
    /* if(!file1.open(QFile::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this,"title","cannot open the file"); */
    /* } */
    /* QTextStream in(&file1); */
    /* QString input_text = in.readLine(); */
    /* int cnt = 0; */
    /* char *buf; */
    /* buf = (char *)calloc(1024, sizeof(char)); */
    /* while(!input_text.isNull()) { */
    /*     buf = input_text.toUtf8().data(); */
    /*     uint32_t num = (uint32_t)strtol(buf, NULL, 2); */
    /*     simu->text_field1[cnt] = (uint8_t)(num >> 24); */
    /*     simu->text_field1[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111)); */
    /*     simu->text_field1[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111)); */
    /*     simu->text_field1[cnt + 3] = (uint8_t)(num & (0b11111111)); */
    /*     input_text = in.readLine(256); */
    /*     cnt += 4; */
    /* } */
    /* file1.close(); */
    /* QFile file(text_file2); */
    /* if(!file2.open(QFile::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this,"title","cannot open the file"); */
    /* } */
    /* QTextStream in(&file2); */
    /* QString input_text = in.readLine(); */
    /* int cnt = 0; */
    /* char *buf; */
    /* buf = (char *)calloc(1024, sizeof(char)); */
    /* while(!input_text.isNull()) { */
    /*     buf = input_text.toUtf8().data(); */
    /*     uint32_t num = (uint32_t)strtol(buf, NULL, 2); */
    /*     simu->text_field2[cnt] = (uint8_t)(num >> 24); */
    /*     simu->text_field2[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111)); */
    /*     simu->text_field2[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111)); */
    /*     simu->text_field2[cnt + 3] = (uint8_t)(num & (0b11111111)); */
    /*     input_text = in.readLine(256); */
    /*     cnt += 4; */
    /* } */
    /* file2.close(); */
    /* QFile file(text_file3); */
    /* if(!file3.open(QFile::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this,"title","cannot open the file"); */
    /* } */
    /* QTextStream in(&file3); */
    /* QString input_text = in.readLine(); */
    /* int cnt = 0; */
    /* char *buf; */
    /* buf = (char *)calloc(1024, sizeof(char)); */
    /* while(!input_text.isNull()) { */
    /*     buf = input_text.toUtf8().data(); */
    /*     uint32_t num = (uint32_t)strtol(buf, NULL, 2); */
    /*     simu->text_field3[cnt] = (uint8_t)(num >> 24); */
    /*     simu->text_field3[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111)); */
    /*     simu->text_field3[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111)); */
    /*     simu->text_field3[cnt + 3] = (uint8_t)(num & (0b11111111)); */
    /*     input_text = in.readLine(256); */
    /*     cnt += 4; */
    /* } */
    /* file3.close(); */

    /* QFile file_d(data_file); */
    /* if(!file_d.open(QFile::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this,"title","cannot open the file"); */
    /* } */
    /* QTextStream in_d(&file_d); */
    /* QString input_text_d = in_d.readLine(); */
    /* int cnt_d = 0; */
    /* char *buf_d; */
    /* buf_d = (char *)calloc(1024, sizeof(char)); */

    /* while(!input_text_d.isNull()) { */
    /*     buf_d = input_text_d.toUtf8().data(); */
    /*     uint32_t num = (uint32_t)strtol(buf_d, NULL, 2); */
    /*     simu->data_field[cnt_d] = (uint8_t)(num >> 24); */
    /*     simu->data_field[cnt_d + 1] = (uint8_t)((num >> 16) & (0b11111111)); */
    /*     simu->data_field[cnt_d + 2] = (uint8_t)((num >> 8) & (0b11111111)); */
    /*     simu->data_field[cnt_d + 3] = (uint8_t)(num & (0b11111111)); */
    /*     input_text_d = in_d.readLine(256); */
    /*     cnt_d += 4; */
    /* } */
    /* display_last_stacks(simu); */
    /* file_d.close(); */
}


void MainWindow::on_pushButton_Next_released()
{
    int loop_num = 0;
    while(loop_num < next_step) {
        execOneInstruction(simu);

        // TODO: Adapt this to Parallel mode
        if(l_lis.now_node->next == l_lis.boss) {
            l_lis.create_new(simu);
            l_lis.siz++;
            if(l_lis.siz > l_lis.mx_siz) {
                l_lis.boss->next = l_lis.boss->next->next;
                free(l_lis.boss->next->prev->stack[0]);
                free(l_lis.boss->next->prev);
                l_lis.boss->next->prev = l_lis.boss;
                l_lis.siz--;
            }
        } else {
            l_lis.now_node = l_lis.now_node->next;
            l_lis.change_simu(simu);
        }

        loop_num++;
    }
    heigh_light_row((simu->pc[0] / 4) * 3);
    update_register_table();
    update_memory_table();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    next_step = arg1;
}

void MainWindow::on_pushButton_Restart_released()
{
    /* heigh_light_row(0); */

    /* memset(simu->registers, 0, sizeof(simu->registers)); */
    /* memset(simu->registers_f, 0, sizeof(simu->registers_f)); */
    /* memset(simu->condition_code, 0, sizeof(simu->condition_code)); */
    /* memset(simu->text_field0, 0, sizeof(uint8_t) * TEXT_SIZE); */
    /* memset(simu->stack_field, 0, sizeof(uint8_t) * STACK_SIZE); */
    /* for (int i=0; i<THREAD_NUM; i++) simu->pc[i] = 0; */

    /* QFile file(file_name); */
    /* if(!file.open(QFile::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this,"title","cannot open the file"); */
    /* } */
    /* QTextStream in(&file); */
    /* QString input_text = in.readLine(); */
    /* int cnt = 0; */
    /* char *buf; */
    /* buf = (char *)calloc(1024, sizeof(char)); */

    /* while(!input_text.isNull()) { */
    /*     buf = input_text.toUtf8().data(); */
    /*     uint32_t num = (uint32_t)strtol(buf, NULL, 2); */
    /*     /1* simu->text_field[cnt] = (uint8_t)(num >> 24); *1/ */
    /*     /1* simu->text_field[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111)); *1/ */
    /*     /1* simu->text_field[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111)); *1/ */
    /*     /1* simu->text_field[cnt + 3] = (uint8_t)(num & (0b11111111)); *1/ */
    /*     input_text = in.readLine(256); */
    /*     cnt += 4; */
    /* } */
    /* file.close(); */

    /* QFile file_d(data_file); */
    /* if(!file_d.open(QFile::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this,"title","cannot open the file"); */
    /* } */
    /* QTextStream in_d(&file_d); */
    /* QString input_text_d = in_d.readLine(); */
    /* int cnt_d = 0; */
    /* char *buf_d; */
    /* buf_d = (char *)calloc(1024, sizeof(char)); */

    /* while(!input_text_d.isNull()) { */
    /*     buf_d = input_text_d.toUtf8().data(); */
    /*     uint32_t num = (uint32_t)strtol(buf_d, NULL, 2); */
    /*     simu->data_field[cnt_d] = (uint8_t)(num >> 24); */
    /*     simu->data_field[cnt_d + 1] = (uint8_t)((num >> 16) & (0b11111111)); */
    /*     simu->data_field[cnt_d + 2] = (uint8_t)((num >> 8) & (0b11111111)); */
    /*     simu->data_field[cnt_d + 3] = (uint8_t)(num & (0b11111111)); */
    /*     input_text_d = in_d.readLine(256); */
    /*     cnt_d += 4; */
    /* } */
    /* file_d.close(); */

    /* l_lis.delete_whole_l(); */

    /* InitialTableDisplay(); */

    /* display_last_stacks(simu); */
}

void MainWindow::on_pushButton_Back_released()
{
    int loop_num = 0;
    while(loop_num < next_step) {

        if(l_lis.now_node == l_lis.boss) {
            return ;
        } else {
            l_lis.now_node = l_lis.now_node->prev;
        }

        back_simu(l_lis.now_node);

        loop_num++;
    }
    heigh_light_row(simu->pc[0] / 4);
    update_register_table();
    update_memory_table();
}

void MainWindow::on_pushButton_All_released()
{

    long long int num_instructions = 0;
    uint32_t pre_pc = -1;

    while(1) {
        execOneInstruction(simu);

        if(pre_pc == simu->pc[0] && get_opcode(simu, 0, 0) != 0b111111) break;
        pre_pc = simu->pc[0];

        num_instructions++;
    }

    //debug
    /* for (int t = 0; t < THREAD_NUM; t++){ */
    /*     printf("thread: %d\n", t); */
    /*     for (Spec *s = simu->spec_list[t]; s != NULL; s = s->next) { */
    /*         printf("addr: %x\n", s->addr); */
    /*         printf("data: %x\n", s->data); */
    /*     } */
    /* } */

    heigh_light_row((simu->pc[0] / 4) * 3);
    display_last_register(simu);
    display_last_data(simu);
    qDebug() << "num_insructions " << num_instructions;
}

void MainWindow::on_spinBox_Nbp_valueChanged(int arg1)
{
    next_break_point = arg1 * 4 / 3;
}

void MainWindow::on_pushButton_Nbp_released()
{
    long long int pre_pc = -1, num_instructions = 0;

    bool same = false;
    if(next_break_point == simu->pc[0]) same = true;

    while(1) {
        execOneInstruction(simu);

        printf("bp, pc: %X %X\n", next_break_point, simu->pc[0]);

        if(pre_pc == simu->pc[0] && get_opcode(simu, 0, 0) != 0b111111) break;
        pre_pc = simu->pc[0];

        if(!same && next_break_point == simu->pc[0]) break;
        same = false;

        num_instructions++;
    }

    memory_dump(simu);

    heigh_light_row((simu->pc[0] / 4) * 3);
    display_last_register(simu);
    display_last_stacks(simu);
    /* l_lis.delete_whole_l(); */
    l_lis.create_new(simu);
    l_lis.siz++;
}

void MainWindow::on_spinBox_StopAt_valueChanged(int arg1)
{
    stop_at = arg1;
}

void MainWindow::on_pushButton_StopAt_released()
{
    long long int pre_pc = -1, cnt_inst = 0;

    while(1) {
        execOneInstruction(simu);

        printf("pc: %X\n", simu->pc[0]);
        if(pre_pc == simu->pc[0] && get_opcode(simu, 0, 0) != 0b111111) break;
        pre_pc = simu->pc[0];

        if(cnt_inst == stop_at) break;

        cnt_inst++;
    }

    heigh_light_row(simu->pc[0] / 4);
    display_last_register(simu);
    /* display_last_stacks(simu); */
    display_last_data(simu);
    /* l_lis.delete_whole_l(); */
    l_lis.create_new(simu);
    l_lis.siz++;
}

void MainWindow::on_pushButton_SwStop_released()
{
    long long int pre_pc = -1;

    while(1) {
        execOneInstruction(simu);

        if(pre_pc == simu->pc[0] && get_opcode(simu, 0, 0) != 0b111111) break;
        pre_pc = simu->pc[0];

        for (int i=0; i<THREAD_NUM; i++) {
            if(get_opcode(simu, i, 0) == 0b101011) {
                uint32_t rs = get_rs(simu, i, 0);
                int32_t imm = get_imm(simu, i, 0);
                if(simu->registers[0][rs] + imm == sw_stop) {
                    goto end;
                }
            }
        }
    }
end:

    heigh_light_row(simu->pc[0] / 4);
    display_last_register(simu);
    /* display_last_stacks(simu); */
    display_last_data(simu);
    /* l_lis.delete_whole_l(); */
    l_lis.create_new(simu);
    l_lis.siz++;
}
void MainWindow::on_spinBox_SwStop_valueChanged(int arg1)
{
    sw_stop = arg1;
}
