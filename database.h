#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
class NewPageItem;
class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    QSqlDatabase db;
    QSqlQuery query(QString in);
    QSqlQuery prepare(QString in);
    QList<NewPageItem> selectFromNewPage(QString cond);
    void insertIntoNewPage(NewPageItem it);
signals:

public slots:
};

#endif // DATABASE_H
