#include "mainwindow.h"
#include <QFormLayout>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , encryptor(new ImageEncryptor(this))
{
    setupUI();
    setWindowTitle("Raw Image Encryptor");
    resize(600, 400);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    createFileGroup();
    createEncryptionGroup();
    createKeyGroup();
    createActionButtons();

    progressBar = new QProgressBar();
    progressBar->setVisible(false);

    mainLayout->addWidget(createFileGroup());
    mainLayout->addWidget(createEncryptionGroup());
    mainLayout->addWidget(createKeyGroup());
    mainLayout->addWidget(createActionButtons());
    mainLayout->addWidget(progressBar);

    setCentralWidget(centralWidget);

    // Connect signals
    connect(encryptor, &ImageEncryptor::progressChanged,
            progressBar, &QProgressBar::setValue);
    connect(encryptor, &ImageEncryptor::operationComplete,
            this, [this](bool success, const QString &message) {
                progressBar->setVisible(false);
                if (success) {
                    QMessageBox::information(this, "Success", message);
                } else {
                    QMessageBox::critical(this, "Error", message);
                }
            });
}

QGroupBox* MainWindow::createFileGroup()
{
    QGroupBox *fileGroup = new QGroupBox("Image Files");
    QFormLayout *layout = new QFormLayout;

    inputFileEdit = new QLineEdit();
    browseInputBtn = new QPushButton("Browse...");
    connect(browseInputBtn, &QPushButton::clicked, this, &MainWindow::browseInputFile);

    outputFileEdit = new QLineEdit();
    browseOutputBtn = new QPushButton("Browse...");
    connect(browseOutputBtn, &QPushButton::clicked, this, &MainWindow::browseOutputFile);

    layout->addRow("Input Image:", createBrowseRow(inputFileEdit, browseInputBtn));
    layout->addRow("Output File:", createBrowseRow(outputFileEdit, browseOutputBtn));

    fileGroup->setLayout(layout);
    return fileGroup;
}

QHBoxLayout* MainWindow::createBrowseRow(QLineEdit *edit, QPushButton *button)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(edit);
    layout->addWidget(button);
    return layout;
}

QGroupBox* MainWindow::createEncryptionGroup()
{
    QGroupBox *encGroup = new QGroupBox("Encryption Settings");
    QFormLayout *layout = new QFormLayout;

    algorithmCombo = new QComboBox();
    algorithmCombo->addItems({"AES-256", "ChaCha20", "XOR"});
    connect(algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateAlgorithm);

    compressCheck = new QCheckBox("Compress before encryption");
    compressCheck->setChecked(true);

    layout->addRow("Algorithm:", algorithmCombo);
    layout->addRow("", compressCheck);

    encGroup->setLayout(layout);
    return encGroup;
}

QGroupBox* MainWindow::createKeyGroup()
{
    QGroupBox *keyGroup = new QGroupBox("Encryption Key");
    QFormLayout *layout = new QFormLayout;

    keyFileEdit = new QLineEdit();
    browseKeyBtn = new QPushButton("Browse...");
    connect(browseKeyBtn, &QPushButton::clicked, this, &MainWindow::browseKeyFile);

    passwordCheck = new QCheckBox("Use password instead");
    connect(passwordCheck, &QCheckBox::toggled, this, [this](bool checked) {
        keyFileEdit->setEnabled(!checked);
        browseKeyBtn->setEnabled(!checked);
        passwordEdit->setEnabled(checked);
    });

    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setEnabled(false);
    QPushButton *showPasswordBtn = new QPushButton("👁");
    showPasswordBtn->setFixedWidth(30);
    connect(showPasswordBtn, &QPushButton::clicked, this, &MainWindow::togglePasswordVisibility);

    QHBoxLayout *passwordLayout = new QHBoxLayout;
    passwordLayout->addWidget(passwordEdit);
    passwordLayout->addWidget(showPasswordBtn);

    layout->addRow("Key File:", createBrowseRow(keyFileEdit, browseKeyBtn));
    layout->addRow("", passwordCheck);
    layout->addRow("Password:", passwordLayout);

    keyGroup->setLayout(layout);
    return keyGroup;
}

QWidget* MainWindow::createActionButtons()
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);

    generateKeyBtn = new QPushButton("Generate New Key");
    encryptBtn = new QPushButton("🔒 Encrypt");
    decryptBtn = new QPushButton("🔓 Decrypt");

    generateKeyBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");
    encryptBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; }");
    decryptBtn->setStyleSheet("QPushButton { background-color: #FF9800; color: white; }");

    connect(generateKeyBtn, &QPushButton::clicked, this, &MainWindow::generateKey);
    connect(encryptBtn, &QPushButton::clicked, this, &MainWindow::encryptImage);
    connect(decryptBtn, &QPushButton::clicked, this, &MainWindow::decryptImage);

    layout->addWidget(generateKeyBtn);
    layout->addWidget(encryptBtn);
    layout->addWidget(decryptBtn);

    return widget;
}

void MainWindow::browseInputFile()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Raw Image File",
                                                "", "Raw Images (*.raw *.dng *.cr2 *.nef *.arw);;All Files (*)");
    if (!file.isEmpty()) {
        inputFileEdit->setText(file);
        if (outputFileEdit->text().isEmpty()) {
            QFileInfo info(file);
            outputFileEdit->setText(info.path() + "/" + info.baseName() + ".enc");
        }
    }
}

void MainWindow::browseOutputFile()
{
    QString file = QFileDialog::getSaveFileName(this, "Save Encrypted File",
                                                "", "Encrypted Files (*.enc *.eimg);;All Files (*)");
    if (!file.isEmpty()) {
        outputFileEdit->setText(file);
    }
}

void MainWindow::browseKeyFile()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Key File",
                                                "", "Key Files (*.key *.bin);;All Files (*)");
    if (!file.isEmpty()) {
        keyFileEdit->setText(file);
    }
}

void MainWindow::encryptImage()
{
    if (inputFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select an input image file.");
        return;
    }

    if (outputFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please specify an output file.");
        return;
    }

    progressBar->setVisible(true);
    progressBar->setValue(0);

    if (passwordCheck->isChecked()) {
        encryptor->encryptWithPassword(inputFileEdit->text(),
                                       outputFileEdit->text(),
                                       passwordEdit->text(),
                                       compressCheck->isChecked());
    } else {
        encryptor->encryptWithKeyFile(inputFileEdit->text(),
                                      outputFileEdit->text(),
                                      keyFileEdit->text(),
                                      compressCheck->isChecked());
    }
}

void MainWindow::decryptImage()
{
    if (inputFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select an input file to decrypt.");
        return;
    }

    progressBar->setVisible(true);
    progressBar->setValue(0);

    if (passwordCheck->isChecked()) {
        encryptor->decryptWithPassword(inputFileEdit->text(),
                                       outputFileEdit->text(),
                                       passwordEdit->text());
    } else {
        encryptor->decryptWithKeyFile(inputFileEdit->text(),
                                      outputFileEdit->text(),
                                      keyFileEdit->text());
    }
}

void MainWindow::generateKey()
{
    QString keyFile = QFileDialog::getSaveFileName(this, "Save Key File",
                                                   "", "Key Files (*.key);;All Files (*)");
    if (!keyFile.isEmpty()) {
        if (encryptor->generateKeyFile(keyFile)) {
            keyFileEdit->setText(keyFile);
            QMessageBox::information(this, "Success", "New key generated successfully!");
        }
    }
}

void MainWindow::updateAlgorithm(int index)
{
    // Update encryption algorithm
    encryptor->setAlgorithm(static_cast<ImageEncryptor::Algorithm>(index));
}

void MainWindow::togglePasswordVisibility()
{
    if (passwordEdit->echoMode() == QLineEdit::Password) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
    }
}
