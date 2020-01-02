#include "database.h"
#include <debug.h>
#include <QSqlError>
#include <QSqlQuery>
#include <newpagemodel.h>

Database::Database(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    db.open();
    D_DATABASE if (!db.isOpen())
        qDebug() << db.lastError().text();
    QStringList list = db.tables();
    for (auto t : list)
    D_DATABASE qDebug() << t << "\n";
      //  QSqlQuery ().exec("drop table settings");
    if(!list.contains("newPage"))
    {
        query("create table newPage ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "pageName VARCHAR, "
                   "time DATETIME, "
                   "fileName VARCHAR,"
                   "del bit)"
                   );
    }
    if(!list.contains("settings"))
    {
        query("create table settings ("
                   "timeLastOk DATETIME, "
                   "timeStartLastAct DATETIME, "
                   "actPeriod INTEGER)"
                   );
    }
    /*{
        QSqlQuery dotaz;
        dotaz.exec("insert into newPage (pageName,time,fileName,del) "
                   "values ('abcFF','2022-03-03 12:33:44.555','def',0)");
        D_DATABASE qDebug() << db.lastError().text();
        //insert into newPage (pageName,time,fileName,del)values ('is-suplovani','2019-04-22 20:53:53.202','is-suplovani--19-04-22-20-53-53.html',0)
    }*/

}
QSqlQuery Database::query(QString in)
{
    QSqlQuery dotaz;
    D_DATABASE qDebug() << "DB" << in;
    if(!dotaz.exec(in))
    D_DATABASE qDebug() << "DB ERROR -----> " << db.lastError().text();
    return dotaz;
}
QSqlQuery Database::prepare(QString in)
{
    QSqlQuery dotaz;
    D_DATABASE qDebug() << "DB PREPARE " << in;
    if(!dotaz.prepare(in))
    D_DATABASE qDebug() << "DB PREPARE ERROR -----> " << db.lastError().text();
    return dotaz;
}
QList<NewPageItem> Database::selectFromNewPage(QString cond)
{
    QSqlQuery dotaz = query("select * from newPage "+cond);
    QList<NewPageItem> out;
    while (dotaz.next())
    {
        NewPageItem in;
        in.pageName = dotaz.value("pageName").toString();
        in.time  = dotaz.value("time").toDateTime();
        in.fileName  = dotaz.value("fileName").toString();
        in.del = dotaz.value("del").toBool();
        in.id = dotaz.value("id").toInt();
        out.push_back(in);
    }
    return out;
}
void Database::insertIntoNewPage(NewPageItem it)
{
    QString q = QString("insert into newPage (pageName,time,fileName,del) ")
        +"values ('"+it.pageName
        +"','"+it.time.toString("yyyy-MM-dd hh:mm:ss.zzz")
        +"','"+it.fileName
        +"',"+QString::number(it.del)+")";
    query(q);
}
