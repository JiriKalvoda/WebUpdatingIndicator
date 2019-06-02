#ifndef PAGECOMPARATORGUI_H
#define PAGECOMPARATORGUI_H

#include <QObject>
#include <QWidget>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QFile>
#include <newpagemodel.h>
#include <QString>
#include <pagecomparator.h>
#include <QPushButton>
class Background;
class PageComparatorGui : public QWidget
{
    Q_OBJECT
public:
    explicit PageComparatorGui(int a,int b,Background * bg,QWidget *parent = 0);
    QHBoxLayout * l;
    PageComparator comp;
    QPushButton * button_generate;
    QPushButton * button_open;
signals:

public slots:
};

#endif // PAGECOMPARATORGUI_H
