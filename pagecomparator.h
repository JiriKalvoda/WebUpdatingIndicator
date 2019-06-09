#ifndef PAGECOMPARATOR_H
#define PAGECOMPARATOR_H

#include <QObject>
#include <QString>
#include <background.h>
#include <QSet>
#include <QStringList>
#include <newpagemodel.h>

class PageComparator : public QObject
{
    Q_OBJECT
public:
    explicit PageComparator(int a,int b,Background * Bg,QObject *parent = 0);
    virtual ~PageComparator();
    Background * bg;
    int id_a, id_b;
    NewPageItem page_a, page_b;
    QString filePrefix;
    QString fileDirPrefix;
    QStringList data;
    QList<int> dataFile;
    int pageWriteiterator=0;
    QSet<QString> openFile;
    void setNewPrefix();
    QStringList parseData(QString in);
    QStringList loadFile(NewPageItem id);
    void generateHeadHtml(char ab,NewPageItem  npit);
    QString generateIframe(int id, bool viewSC);
public slots:
    void generate(bool viewSC);
    void open();
    void load();
    void calculatePages(int id_a,int id_b);
};

#endif // PAGECOMPARATOR_H
