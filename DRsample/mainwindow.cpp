#include "mainwindow.h"

mainwindow::mainwindow()
{
    QString dbName = "DR";
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("DRIVER={SQL Server};Server=(local);Database="+dbName+";Uid=sa;Pwd=panacea");
    if(db.open())
    {
        qDebug()<<"db is open";
        QSqlQuery* Query = new QSqlQuery();

        Query->prepare("SELECT * FROM worklist ");
        Query->exec();
        while (Query->next()) {
            QString output;
            for (int i = 0; i < Query->record().count(); ++i) {
                QString fieldName = Query->record().fieldName(i);
                QVariant value = Query->value(i);
                output += fieldName + ": " + value.toString() + " | ";
            }
            qDebug() << output;
        }
    }
}
