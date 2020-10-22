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

using namespace std;

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
    form = new Form;
    form->show();
    InitialTableDisplay();

    init_instructions();

    simu = create_simu(MEMORY_SIZE, 0, 0);

    for(int i = 0; i < 5; i++) {
        simu_ary[i] = create_simu(MEMORY_SIZE, 0, 0);
    }

    for(int i = 0; i < 33; i++) {
        ui->tableWidget_Registers->setRowHeight(i, 29);
    }

    for(int i = 0; i < 33; i++) {
        ui->tableWidget_Memory->setRowHeight(i, 29);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Quit_released()
{
    form->close();
}

void MainWindow::InitialTableDisplay()
{

    ui->tableWidget_Memory->setRowCount(33);
    ui->tableWidget_Memory->setColumnCount(5);

    QStringList vlabels2;
    vlabels2 << "ZERO" << "AT" << "V0" << "V1" << "A0" << "A1" << "A2" << "A3" << "T0" << "T1"
            << "T2" << "T3" << "T4" << "T5" << "T6" << "T7" << "S0" << "S1" << "S2" << "S3"
            << "S4" << "S5" << "S6" << "S7" << "T8" << "T9" << "K0" << "K1" << "GP" << "SP" << "FP" << "RA" << "N_pc";
    ui->tableWidget_Memory->setVerticalHeaderLabels(vlabels2);

    QStringList hlabels2;
    hlabels2 << "prepre" << "pre" << "now" << "next" << "nextnext";
    ui->tableWidget_Memory->setHorizontalHeaderLabels(hlabels2);

    for(int i = 0; i < 33; i++) {
        for (int j = 0; j < 5; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(0, 16));
            ui->tableWidget_Memory->setItem(i, j, item);
        }
    }

    ui->tableWidget_Memory->setAlternatingRowColors(true);

    ui->tableWidget_Registers->setRowCount(33);
    ui->tableWidget_Registers->setColumnCount(5);

    QStringList vlabels;
    vlabels << "ZERO" << "AT" << "V0" << "V1" << "A0" << "A1" << "A2" << "A3" << "T0" << "T1"
            << "T2" << "T3" << "T4" << "T5" << "T6" << "T7" << "S0" << "S1" << "S2" << "S3"
            << "S4" << "S5" << "S6" << "S7" << "T8" << "T9" << "K0" << "K1" << "GP" << "SP" << "FP" << "RA" << "N_pc";
    ui->tableWidget_Registers->setVerticalHeaderLabels(vlabels);

    QStringList hlabels;
    hlabels << "prepre" << "pre" << "now" << "next" << "nextnext";
    ui->tableWidget_Registers->setHorizontalHeaderLabels(hlabels);

    for(int i = 0; i < 33; i++) {
        for (int j = 0; j < 5; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(0, 16));
            ui->tableWidget_Registers->setItem(i, j, item);
        }
    }

    ui->tableWidget_Registers->setAlternatingRowColors(true);

}

void MainWindow::update_register_table()
{
    for(int i = 0; i < 5; i++) {
        for (int j = 0; j < 32; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QString::number(simu_ary[i]->registers[j], 16));
            ui->tableWidget_Registers->setItem(j, i, item);
        }
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(simu_ary[i]->pc, 16));
        ui->tableWidget_Registers->setItem(32, i, item);
    }

    for(int i = 0; i < 32; i++) {
        if(ui->tableWidget_Registers->item(i, 1)->text() != ui->tableWidget_Registers->item(i, 2)->text()) {
            ui->tableWidget_Registers->item(i, 1)->setBackground(Qt::red);
            ui->tableWidget_Registers->item(i, 2)->setBackground(Qt::green);
        }
    }
    if(ui->tableWidget_Registers->item(32, 1)->text() != ui->tableWidget_Registers->item(32, 2)->text()) {
        ui->tableWidget_Registers->item(32, 1)->setBackground(Qt::red);
        ui->tableWidget_Registers->item(32, 2)->setBackground(Qt::green);
    }
}

void MainWindow::shift_simu_ary_next()
{
    for (int i = 0; i < 2; i++) {
        simu_ary[i]->pc = simu_ary[i + 1]->pc;
        for (int j = 0; j < 32; j++) {
            simu_ary[i]->registers[j] = simu_ary[i + 1]->registers[j];
        }

        //　メモリはいったんコピらない
    }

    simu_ary[2]->pc = simu->pc;
    for (int j = 0; j < 32; j++) {
        simu_ary[2]->registers[j] = simu->registers[j];
    }
}

void MainWindow::on_pushButton_Open_released()
{
    file_name = QFileDialog::getOpenFileName(this, "open a file","");

    form->displayFile(file_name);

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
        shift_simu_ary_next();
        update_register_table();
        form->heigh_light_row(simu->pc / 4);
        loop_num++;
    }

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    next_step = arg1;
}

void MainWindow::on_pushButton_Restart_released()
{
    form->heigh_light_row(0);

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

    for (int i = 0; i < 5; i++) {
        simu_ary[i] = create_simu(MEMORY_SIZE, 0, 0);
    }

    InitialTableDisplay();
}
