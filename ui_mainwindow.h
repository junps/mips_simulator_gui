/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableWidget_Registers = new QTableWidget(centralwidget);
        tableWidget_Registers->setObjectName(QStringLiteral("tableWidget_Registers"));

        horizontalLayout->addWidget(tableWidget_Registers);

        tableWidget_Memory = new QTableWidget(centralwidget);
        tableWidget_Memory->setObjectName(QStringLiteral("tableWidget_Memory"));

        horizontalLayout->addWidget(tableWidget_Memory);

        listWidget_Code = new QListWidget(centralwidget);
        listWidget_Code->setObjectName(QStringLiteral("listWidget_Code"));

        horizontalLayout->addWidget(listWidget_Code);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMaximum(100000000);
        spinBox->setValue(1);

        horizontalLayout_2->addWidget(spinBox);

        pushButton_All = new QPushButton(centralwidget);
        pushButton_All->setObjectName(QStringLiteral("pushButton_All"));

        horizontalLayout_2->addWidget(pushButton_All);

        pushButton_Next = new QPushButton(centralwidget);
        pushButton_Next->setObjectName(QStringLiteral("pushButton_Next"));

        horizontalLayout_2->addWidget(pushButton_Next);

        pushButton_Back = new QPushButton(centralwidget);
        pushButton_Back->setObjectName(QStringLiteral("pushButton_Back"));

        horizontalLayout_2->addWidget(pushButton_Back);

        pushButton_Open = new QPushButton(centralwidget);
        pushButton_Open->setObjectName(QStringLiteral("pushButton_Open"));

        horizontalLayout_2->addWidget(pushButton_Open);

        pushButton_Restart = new QPushButton(centralwidget);
        pushButton_Restart->setObjectName(QStringLiteral("pushButton_Restart"));

        horizontalLayout_2->addWidget(pushButton_Restart);

        pushButton_Quit = new QPushButton(centralwidget);
        pushButton_Quit->setObjectName(QStringLiteral("pushButton_Quit"));

        horizontalLayout_2->addWidget(pushButton_Quit);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton_Quit, SIGNAL(released()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton_All->setText(QApplication::translate("MainWindow", "All", Q_NULLPTR));
        pushButton_Next->setText(QApplication::translate("MainWindow", "Next", Q_NULLPTR));
        pushButton_Back->setText(QApplication::translate("MainWindow", "Back", Q_NULLPTR));
        pushButton_Open->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        pushButton_Restart->setText(QApplication::translate("MainWindow", "Restart", Q_NULLPTR));
        pushButton_Quit->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
