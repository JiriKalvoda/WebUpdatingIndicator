#ifndef PAGECOMPARATOR_H
#define PAGECOMPARATOR_H

#include <QObject>
#include <QString>
#include <background.h>
#include <QSet>
#include <QStringList>

class PageComparator : public QObject
{
    Q_OBJECT
public:
    explicit PageComparator(int a,int b,Background * Bg,QObject *parent = 0);
    virtual ~PageComparator();
    Background * bg;
    int id_a, id_b;
    QString filePrefix;
    QString fileDirPrefix;
    QStringList data;
    QList<int> dataFile;

    QSet<QString> openFile;
    void setNewPrefix();
    QStringList parseData(QString in);
    QStringList loadFile(int id);
public slots:
    void generate();
    void open();
    void load();
    void calculatePages(int id_a,int id_b);
};

#endif // PAGECOMPARATOR_H