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


class PageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit PageViewer(QWidget *parent = 0);
    QTableView *table;
    QVBoxLayout *l;
    QHBoxLayout *buttons;
    QPushButton *button_hide, *button_delete, *button_compare;
    QLabel * buttons_middleLabel;
    virtual void setModel(QAbstractItemModel *model);
    QSortFilterProxyModel *sort;
signals:

public slots:
    void clicked(QModelIndex index);
};

#endif // DELETETABLEVIEWER_H
