#include "mainwindow.h"
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

#define MEMORY_SIZE (2000000000)

#define _NOW 2
#define _PRE 1
#define _PREPRE 0
#define _NEXT 3
#define _NEXTNEXT 4

string registers_name[] = { "ZERO", "AT", "V0", "V1", "A0", "A1", "A2", "A3", "T0", "T1",
                            "T2", "T3", "T4", "T5", "T6", "T7", "S0", "S1", "S2", "S3", "S4",
                            "S5", "S6", "S7", "T8", "T9", "K0", "K1", "GP", "SP", "FP", "RA" };

Simulator* create_simu(size_t size, uint32_t pc, uint32_t sp) {
    Simulator* simu = (Simulator*)malloc(sizeof(Simulator));
    simu->memory = (uint8_t*)malloc(size);

    memset(simu->registers, 0, sizeof(simu->registers));

    simu->pc = pc;
    simu->registers[SP] = sp;
    simu->registers[ZERO] = 0;

    return simu;
}

void destroy_simu(Simulator* simu) {
    free(simu->memory);
    free(simu);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitialTableDisplay();

    init_instructions();

    simu = create_simu(MEMORY_SIZE, 0, 0);

    for(int i = 0; i < 33; i++) {
        ui->tableWidget_Registers->setRowHeight(i, 28);
    }

    for(int i = 0; i < 33; i++) {
        ui->tableWidget_Memory->setRowHeight(i, 28);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::InitialTableDisplay()
{
    ui->tableWidget_Registers->setRowCount(33);
    ui->tableWidget_Registers->setColumnCount(3);

    QStringList vlabels;
    vlabels << "ZERO" << "AT" << "V0" << "V1" << "A0" << "A1" << "A2" << "A3" << "T0" << "T1"
            << "T2" << "T3" << "T4" << "T5" << "T6" << "T7" << "S0" << "S1" << "S2" << "S3"
            << "S4" << "S5" << "S6" << "S7" << "T8" << "T9" << "K0" << "K1" << "GP" << "SP" << "FP" << "RA" << "N_pc";
    ui->tableWidget_Registers->setVerticalHeaderLabels(vlabels);

    QStringList hlabels;
    hlabels << "pre" << "now" << "next";
    ui->tableWidget_Registers->setHorizontalHeaderLabels(hlabels);

    for(int i = 0; i < 33; i++) {
        for (int j = 0; j < 3; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(0, 16));
            ui->tableWidget_Registers->setItem(i, j, item);
        }
    }

    ui->tableWidget_Registers->setAlternatingRowColors(true);

    ui->tableWidget_Memory->setRowCount(l_lis.stack_size + 1);
    ui->tableWidget_Memory->setColumnCount(3);

    QStringList vlabels2;
    for(int i = 0; i <= l_lis.stack_size; i++) {
        vlabels2 << QString::number(l_lis.ini_sp + i);
    }
    ui->tableWidget_Memory->setVerticalHeaderLabels(vlabels2);

    QStringList hlabels2;
    hlabels2 << "pre" << "now" << "next";
    ui->tableWidget_Memory->setHorizontalHeaderLabels(hlabels2);

    for(int i = 0; i <= l_lis.ini_sp; i++) {
        for (int j = 0; j < 3; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(0, 16));
            ui->tableWidget_Memory->setItem(i, j, item);
        }
    }

    ui->tableWidget_Memory->setAlternatingRowColors(true);

}

void MainWindow::update_register_table()
{
    // now_node
    for(int i = 0; i < 32; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->registers[i], 16));
        ui->tableWidget_Registers->setItem(i, 1, item);
    }
    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(QString::number(l_lis.now_node->pc, 16));
    ui->tableWidget_Registers->setItem(32, 1, item1);

    // now_node->prev
    for(int i = 0; i < 32; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->prev->registers[i], 16));
        ui->tableWidget_Registers->setItem(i, 0, item);
    }
    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(QString::number(l_lis.now_node->prev->pc, 16));
    ui->tableWidget_Registers->setItem(32, 0, item0);

    //now_node->next
    for(int i = 0; i < 32; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->next->registers[i], 16));
        ui->tableWidget_Registers->setItem(i, 2, item);
    }
    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText(QString::number(l_lis.now_node->next->pc, 16));
    ui->tableWidget_Registers->setItem(32, 2, item2);


    for(int i = 0; i < 32; i++) {
        if(ui->tableWidget_Registers->item(i, 0)->text() != ui->tableWidget_Registers->item(i, 1)->text()) {
            ui->tableWidget_Registers->item(i, 0)->setBackground(Qt::red);
            ui->tableWidget_Registers->item(i, 1)->setBackground(Qt::green);
        }
    }
    if(ui->tableWidget_Registers->item(32, 0)->text() != ui->tableWidget_Registers->item(32, 1)->text()) {
        ui->tableWidget_Registers->item(32, 0)->setBackground(Qt::red);
        ui->tableWidget_Registers->item(32, 1)->setBackground(Qt::green);
    }
}

void MainWindow::update_memory_table()
{
    // now_node
    for(int i = 0; i <= l_lis.stack_size; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->stack[i], 16));
        ui->tableWidget_Memory->setItem(i, 1, item);
    }

    // now_node->prev
    for(int i = 0; i <= l_lis.stack_size; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->prev->stack[i], 16));
        ui->tableWidget_Memory->setItem(i, 0, item);
    }

    //now_node->next
    for(int i = 0; i <= l_lis.stack_size; i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(l_lis.now_node->next->stack[i], 16));
        ui->tableWidget_Memory->setItem(i, 2, item);
    }


    for(int i = 0; i <= l_lis.stack_size; i++) {
        if(ui->tableWidget_Memory->item(i, 0)->text() != ui->tableWidget_Memory->item(i, 1)->text()) {
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


void MainWindow::on_pushButton_Open_released()
{
    file_name = QFileDialog::getOpenFileName(this, "open a file","");

    displayFile(file_name);

    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
    }
    QTextStream in(&file);
    QString input_text = in.readLine();
    int cnt = 0;
    char *buf;
    buf = (char *)calloc(1024, sizeof(char));
    while(!input_text.isNull()) {
        buf = input_text.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf, NULL, 2);
        simu->memory[cnt] = (uint8_t)(num >> 24);
        simu->memory[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->memory[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->memory[cnt + 3] = (uint8_t)(num & (0b11111111));
        input_text = in.readLine(256);
        cnt += 4;
    }
    file.close();
}

void MainWindow::on_pushButton_Next_released()
{
    int loop_num = 0;
    while(loop_num < next_step) {
        uint32_t opcode = get_opcode(simu);
        uint32_t funct = get_func(simu);

        if(opcode == 0b111111) return;

        if (instructions[opcode][funct] == NULL) {
            printf("\n\nNot Implemented: opcode : %x, funct : %x\n", opcode, funct);
            exit(1);
        }

        instructions[opcode][funct](simu);
        heigh_light_row(simu->pc / 4);

        if(l_lis.now_node->next == l_lis.boss) {
            l_lis.create_new(simu);
            l_lis.siz++;
            if(l_lis.siz > l_lis.mx_siz) {
                l_lis.boss->next = l_lis.boss->next->next;
                free(l_lis.boss->next->prev->stack);
                free(l_lis.boss->next->prev);
                l_lis.boss->next->prev = l_lis.boss;
                l_lis.siz--;
            }
        } else {
            l_lis.now_node = l_lis.now_node->next;
        }

        update_register_table();
        update_memory_table();

        loop_num++;
    }

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    next_step = arg1;
}

void MainWindow::on_pushButton_Restart_released()
{
    heigh_light_row(0);

    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","cannot open the file");
    }
    QTextStream in(&file);
    QString input_text = in.readLine();
    int cnt = 0;
    char *buf;
    buf = (char *)calloc(1024, sizeof(char));

    memset(simu->registers, 0, sizeof(simu->registers));
    memset(simu->memory, 0, sizeof(uint8_t) * MEMORY_SIZE);
    simu->pc = 0;

    while(!input_text.isNull()) {
        buf = input_text.toUtf8().data();
        uint32_t num = (uint32_t)strtol(buf, NULL, 2);
        simu->memory[cnt] = (uint8_t)(num >> 24);
        simu->memory[cnt + 1] = (uint8_t)((num >> 16) & (0b11111111));
        simu->memory[cnt + 2] = (uint8_t)((num >> 8) & (0b11111111));
        simu->memory[cnt + 3] = (uint8_t)(num & (0b11111111));
        input_text = in.readLine(256);
        cnt += 4;
    }
    file.close();

    l_lis.delete_whole_l();

    InitialTableDisplay();
}

//void MainWindow::on_pushButton_Back_released()
//{
//    int loop_num = 0;
//    while(loop_num < next_step) {
//        uint32_t opcode = get_opcode(simu);
//        uint32_t funct = get_func(simu);

//        if(opcode == 0b111111) return;

//        if (instructions[opcode][funct] == NULL) {
//            printf("\n\nNot Implemented: opcode : %x, funct : %x\n", opcode, funct);
//            exit(1);
//        }

//        instructions[opcode][funct](simu);
//        form->heigh_light_row(simu->pc / 4);

//        if(l_lis.now_node->next == l_lis.boss) {
//            l_lis.create_new(simu);
//            l_lis.siz++;
//            if(l_lis.siz > l_lis.mx_siz) {
//                l_lis.boss->next = l_lis.boss->next->next;
//                free(l_lis.boss->next->prev->stack);
//                free(l_lis.boss->next->prev);
//                l_lis.boss->next->prev = l_lis.boss;
//                l_lis.siz--;
//            }
//        } else {
//            l_lis.now_node = l_lis.now_node->next;
//        }

//        update_register_table();
//        update_memory_table();

//        loop_num++;
//    }
//}
