#include "pageviewer.h"
#include <QHeaderView>
#include <qDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>


PageViewer::PageViewer(QWidget *parent) : QWidget(parent)
{
    sort.setSortRole(Qt::UserRole);
    table.setModel(&sort);

    table.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //table.setItemDelegate(0);
    table.setSortingEnabled(1);

    connect(&table,SIGNAL(pressed(QModelIndex)),this,SLOT(clicked(QModelIndex)));

    l.addWidget(&table);
    setLayout(&l);

}
void PageViewer::setModel(QAbstractItemModel *model)
{
    sort.setSourceModel(model);
}
void PageViewer::clicked(QModelIndex index)
{
    qDebug ( ) << "clicked" << index.column() <<" "<< index.row();
    if(index.column()== 1)
    {
        qDebug() << QUrl(QString("file:")+QDir::currentPath()+"/history/"+sort.data(index,Qt::DisplayRole).toString());
        QDesktopServices::openUrl(QUrl(QString("file:")+QDir::currentPath()+"/history/"+sort.data(index,Qt::DisplayRole).toString()));
    }
    if(index.column()==sort.columnCount()-1)
    {
        sort.removeRows(index.row(),1);
    }

}
