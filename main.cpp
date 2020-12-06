#include "mainwindow.h"
#include "no_gui.h"

#include <QApplication>
#include <QDebug>

#include <string>

#define MAX_N_WORDS 4096
fi_union sld_words[MAX_N_WORDS];
unsigned sld_n_words = 0;
unsigned sld_index = 0;

int main(int argc, char *argv[])
{
    if(argc == 3) { //no_gui
        string file_path = argv[2];
        QString file_name = QString::fromStdString(file_path);
        //qDebug() << file_name;
        exec_all(file_name, argv[1]);
        return 0;
    } else { //gui
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
