/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget_Registers;
    QTableWidget *tableWidget_Memory;
    QListWidget *listWidget_Code;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QSpinBox *spinBox_StopAt;
    QPushButton *pushButton_StopAt;
    QSpinBox *spinBox_Nbp;
    QPushButton *pushButton_Nbp;
    QSpinBox *spinBox;
    QPushButton *pushButton_All;
    QPushButton *pushButton_Next;
    QPushButton *pushButton_Back;
    QPushButton *pushButton_Open;
    QPushButton *pushButton_Restart;
    QPushButton *pushButton_Quit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(852, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableWidget_Registers = new QTableWidget(centralwidget);
        tableWidget_Registers->setObjectName(QString::fromUtf8("tableWidget_Registers"));

        horizontalLayout->addWidget(tableWidget_Registers);

        tableWidget_Memory = new QTableWidget(centralwidget);
        tableWidget_Memory->setObjectName(QString::fromUtf8("tableWidget_Memory"));

        horizontalLayout->addWidget(tableWidget_Memory);

        listWidget_Code = new QListWidget(centralwidget);
        listWidget_Code->setObjectName(QString::fromUtf8("listWidget_Code"));

        horizontalLayout->addWidget(listWidget_Code);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        spinBox_StopAt = new QSpinBox(centralwidget);
        spinBox_StopAt->setObjectName(QString::fromUtf8("spinBox_StopAt"));
        spinBox_StopAt->setMaximum(1000000000);

        horizontalLayout_2->addWidget(spinBox_StopAt);

        pushButton_StopAt = new QPushButton(centralwidget);
        pushButton_StopAt->setObjectName(QString::fromUtf8("pushButton_StopAt"));

        horizontalLayout_2->addWidget(pushButton_StopAt);

        spinBox_Nbp = new QSpinBox(centralwidget);
        spinBox_Nbp->setObjectName(QString::fromUtf8("spinBox_Nbp"));
        spinBox_Nbp->setMaximum(1000000000);

        horizontalLayout_2->addWidget(spinBox_Nbp);

        pushButton_Nbp = new QPushButton(centralwidget);
        pushButton_Nbp->setObjectName(QString::fromUtf8("pushButton_Nbp"));

        horizontalLayout_2->addWidget(pushButton_Nbp);

        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMaximum(100000000);
        spinBox->setValue(1);

        horizontalLayout_2->addWidget(spinBox);

        pushButton_All = new QPushButton(centralwidget);
        pushButton_All->setObjectName(QString::fromUtf8("pushButton_All"));

        horizontalLayout_2->addWidget(pushButton_All);

        pushButton_Next = new QPushButton(centralwidget);
        pushButton_Next->setObjectName(QString::fromUtf8("pushButton_Next"));

        horizontalLayout_2->addWidget(pushButton_Next);

        pushButton_Back = new QPushButton(centralwidget);
        pushButton_Back->setObjectName(QString::fromUtf8("pushButton_Back"));

        horizontalLayout_2->addWidget(pushButton_Back);

        pushButton_Open = new QPushButton(centralwidget);
        pushButton_Open->setObjectName(QString::fromUtf8("pushButton_Open"));

        horizontalLayout_2->addWidget(pushButton_Open);

        pushButton_Restart = new QPushButton(centralwidget);
        pushButton_Restart->setObjectName(QString::fromUtf8("pushButton_Restart"));

        horizontalLayout_2->addWidget(pushButton_Restart);

        pushButton_Quit = new QPushButton(centralwidget);
        pushButton_Quit->setObjectName(QString::fromUtf8("pushButton_Quit"));

        horizontalLayout_2->addWidget(pushButton_Quit);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 852, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton_Quit, SIGNAL(released()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_StopAt->setText(QApplication::translate("MainWindow", "StopAt", nullptr));
        pushButton_Nbp->setText(QApplication::translate("MainWindow", "N_bp", nullptr));
        pushButton_All->setText(QApplication::translate("MainWindow", "All", nullptr));
        pushButton_Next->setText(QApplication::translate("MainWindow", "Next", nullptr));
        pushButton_Back->setText(QApplication::translate("MainWindow", "Back", nullptr));
        pushButton_Open->setText(QApplication::translate("MainWindow", "Open", nullptr));
        pushButton_Restart->setText(QApplication::translate("MainWindow", "Restart", nullptr));
        pushButton_Quit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
