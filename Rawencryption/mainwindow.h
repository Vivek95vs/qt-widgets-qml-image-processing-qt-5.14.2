#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QProgressBar>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "imageencryptor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseInputFile();
    void browseOutputFile();
    void browseKeyFile();
    void encryptImage();
    void decryptImage();
    void generateKey();
    void updateAlgorithm(int index);
    void togglePasswordVisibility();

private:
    void setupUI();
    void createFileGroup();
    void createEncryptionGroup();
    void createKeyGroup();
    void createActionButtons();

    // UI Components
    QLineEdit *inputFileEdit;
    QLineEdit *outputFileEdit;
    QLineEdit *keyFileEdit;
    QLineEdit *passwordEdit;
    QComboBox *algorithmCombo;
    QCheckBox *compressCheck;
    QCheckBox *passwordCheck;
    QProgressBar *progressBar;

    QPushButton *browseInputBtn;
    QPushButton *browseOutputBtn;
    QPushButton *browseKeyBtn;
    QPushButton *encryptBtn;
    QPushButton *decryptBtn;
    QPushButton *generateKeyBtn;

    // Encryption engine
    ImageEncryptor *encryptor;
};
#endif // MAINWINDOW_H
