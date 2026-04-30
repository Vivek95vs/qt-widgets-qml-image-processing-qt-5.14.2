/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *MotorEnable;
    QPushButton *MotroAutoset;
    QPushButton *Boardconnect;
    QLineEdit *Position;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *Velocity;
    QLabel *label_3;
    QLabel *connectionStatusLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MotorEnable = new QPushButton(centralwidget);
        MotorEnable->setObjectName(QString::fromUtf8("MotorEnable"));
        MotorEnable->setGeometry(QRect(170, 290, 75, 23));
        MotroAutoset = new QPushButton(centralwidget);
        MotroAutoset->setObjectName(QString::fromUtf8("MotroAutoset"));
        MotroAutoset->setGeometry(QRect(260, 290, 91, 23));
        Boardconnect = new QPushButton(centralwidget);
        Boardconnect->setObjectName(QString::fromUtf8("Boardconnect"));
        Boardconnect->setGeometry(QRect(60, 290, 101, 23));
        Position = new QLineEdit(centralwidget);
        Position->setObjectName(QString::fromUtf8("Position"));
        Position->setGeometry(QRect(160, 170, 113, 20));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 170, 81, 21));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 210, 47, 13));
        Velocity = new QLineEdit(centralwidget);
        Velocity->setObjectName(QString::fromUtf8("Velocity"));
        Velocity->setGeometry(QRect(160, 200, 113, 20));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(60, 120, 151, 31));
        connectionStatusLabel = new QLabel(centralwidget);
        connectionStatusLabel->setObjectName(QString::fromUtf8("connectionStatusLabel"));
        connectionStatusLabel->setGeometry(QRect(50, 80, 111, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        MotorEnable->setText(QCoreApplication::translate("MainWindow", "Motor Enable", nullptr));
        MotroAutoset->setText(QCoreApplication::translate("MainWindow", "Motor Autoset", nullptr));
        Boardconnect->setText(QCoreApplication::translate("MainWindow", "Board connect", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Position", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Velocity", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Tube lateral", nullptr));
        connectionStatusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
