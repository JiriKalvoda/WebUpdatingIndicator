#ifndef DELETETABLEVIEWER_H
#define DELETETABLEVIEWER_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QIdentityProxyModel>
#include <QLabel>
#include <newpagemodel.h>
#include <pagecomparatorgui.h>

class Background;

class PageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PageViewer(Background * bg,QWidget *parent = 0);
    NewPageModel * inputModel;
    Background * bg;
    QTableView *table;
    QVBoxLayout *l;
    QHBoxLayout *buttons;
    QPushButton *button_hide, *button_delete, *button_compare;
    QLabel * buttons_middleLabel;
    virtual void setModel(NewPageModel *model);
    QSortFilterProxyModel *sort;
signals:
    void needActualization();
public slots:
    void clicked(QModelIndex index);
    void hideSlot();
    void deleteSlot();
    void compareSlot();
};

#endif // DELETETABLEVIEWER_H
