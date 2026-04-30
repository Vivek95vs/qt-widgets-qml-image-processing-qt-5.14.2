#include <QCoreApplication>
#include "Program.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Program program;
    program.Main();

    return a.exec();
}
