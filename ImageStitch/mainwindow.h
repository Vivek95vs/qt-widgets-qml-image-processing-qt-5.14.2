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

private slots:

    void on_BasedOnJaw_clicked();

    void on_BasedOnField_clicked();

    void on_Blending_pixel_clicked();

    void on_removelinefromstitched_clicked();

    void on_Intensity_correction_clicked();

    void on_intensity_correction2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
