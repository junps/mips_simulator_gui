#include "mainwindow.h"
#include "no_gui.h"

#include <QApplication>
#include <QDebug>

#include <string>

int main(int argc, char *argv[])
{
    if(argc == 2) { //no_gui
        string file_path = argv[1];
        QString file_name = QString::fromStdString(file_path);
        //qDebug() << file_name;
        exec_all(file_name);
        return 0;
    } else { //gui
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
