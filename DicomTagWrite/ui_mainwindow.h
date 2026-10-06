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
    QPushButton *RF_modality;
    QPushButton *SaveRaw;
    QPushButton *XA_modality;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        RF_modality = new QPushButton(centralwidget);
        RF_modality->setObjectName(QString::fromUtf8("RF_modality"));
        RF_modality->setGeometry(QRect(60, 440, 75, 23));
        SaveRaw = new QPushButton(centralwidget);
        SaveRaw->setObjectName(QString::fromUtf8("SaveRaw"));
        SaveRaw->setGeometry(QRect(170, 440, 75, 23));
        XA_modality = new QPushButton(centralwidget);
        XA_modality->setObjectName(QString::fromUtf8("XA_modality"));
        XA_modality->setGeometry(QRect(270, 440, 75, 23));
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
        RF_modality->setText(QCoreApplication::translate("MainWindow", "RF Modality", nullptr));
        SaveRaw->setText(QCoreApplication::translate("MainWindow", "Save Raw", nullptr));
        XA_modality->setText(QCoreApplication::translate("MainWindow", "XA Modality", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
