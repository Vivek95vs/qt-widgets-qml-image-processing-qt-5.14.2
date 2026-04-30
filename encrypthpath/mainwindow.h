#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_decryptpath_clicked();

    void on_Encrypthpath_clicked();

private:
    static bool checkKey(const QString &hexKey) {
        QByteArray key = QByteArray::fromHex(hexKey.toLatin1());
        return (key.size() == 32);
    }
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
