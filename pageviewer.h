#ifndef DELETETABLEVIEWER_H
#define DELETETABLEVIEWER_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QSortFilterProxyModel>
#include <QIdentityProxyModel>

#include "deleteproxy.h"

class PageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PageViewer(QWidget *parent = 0);
    QTableView table;
    QVBoxLayout l;
    virtual void setModel(QAbstractItemModel *model);
    QSortFilterProxyModel sort;
signals:

public slots:
    void clicked(QModelIndex index);
};

#endif // DELETETABLEVIEWER_H
