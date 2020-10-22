#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "form.h"

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

    void on_pushButton_Quit_released();

    void on_pushButton_Next_released();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    Form *form;

    void InitialTableDisplay();

    void update_register_table();

    void shift_simu_ary_next();

    int next_step = 1;
};
#endif // MAINWINDOW_H

