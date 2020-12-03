#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <map>
#include <bitset>

#include "simulator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_Open_released();

    void on_pushButton_Next_released();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_Restart_released();

    void on_pushButton_Back_released();

    void on_pushButton_All_released();

    void on_spinBox_Nbp_valueChanged(int arg1);

    void on_pushButton_Nbp_released();

private:
    Ui::MainWindow *ui;

    void InitialTableDisplay();

    void update_register_table();

    void update_memory_table();

    void shift_simu_ary_next();

    long long int next_step = 1;

    long long int next_break_point = 0;

    QString file_name;

    QString data_file;

    Simulator* simu;

    Simu_linked_list l_lis;

    void displayFile(QString file_name);

    void heigh_light_row(int row);

    void back_simu(Small_simu* small_simu);

    void display_last_register(Simulator* simu);

    void display_last_stacks(Simulator* simu);
};
#endif // MAINWINDOW_H

