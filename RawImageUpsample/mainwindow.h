#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool upsample16BitRawTo3072(const QString &inputPath, const QString &outputPath);
    bool upsample16BitRawManual(const QString &inputPath, const QString &outputPath);
    bool upsample16BitRawWithEndianness(const QString &inputPath, const QString &outputPath, bool isBigEndian);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
