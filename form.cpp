#include "form.h"
#include "ui_form.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QListWidgetItem>

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

using namespace std;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::displayFile(QString file_name)
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
        ui->listWidget->addItem(item);
        input_text = in.readLine(256);
        i++;
    }
    file.close();
    heigh_light_row(0);
}

void Form::heigh_light_row(int row)
{
    ui->listWidget->setCurrentRow(row);
    ui->listWidget->scrollToItem(ui->listWidget->currentItem());
}
