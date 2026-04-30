#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RawImageEncryptor.h"
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Encrypt_clicked()
{
     std::cout << "=== Raw Image Encryption Tool ===\n" << std::endl;

       // Method 1: Use existing key
       std::string myKey = "MySecretKey1234567890!@#$%^&*";
       RawImageEncryptor encryptor(myKey);

       // Or generate random key
       // RawImageEncryptor encryptor; // Auto-generates random key

       // Encrypt a raw image
       std::string inputImage = "image.raw";
       std::string encryptedFile = "image.enc";

       std::cout << "Encrypting file..." << std::endl;
       if (encryptor.encryptFile(inputImage, encryptedFile, RawImageEncryptor::AES_256_CBC)) {
           std::cout << "\nSave this key for decryption: " << std::endl;
           std::cout << encryptor.getKey() << std::endl;

           // Save key to file
           encryptor.saveKeyToFile("encryption.key");
       }

       std::cout << "\n" << std::string(50, '=') << "\n" << std::endl;

       // Decrypt the image
       std::string decryptedImage = "image_decrypted.raw";

       std::cout << "Decrypting file..." << std::endl;
       if (encryptor.decryptFile(encryptedFile, decryptedImage, RawImageEncryptor::AES_256_CBC)) {
           std::cout << "Image successfully decrypted!" << std::endl;
       }

}
