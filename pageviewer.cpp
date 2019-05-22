#include "pageviewer.h"
#include <QHeaderView>
#include <qDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>


PageViewer::PageViewer(QWidget *parent) : QWidget(parent)
{
    table = new QTableView;
    l = new QVBoxLayout;
    buttons = new QHBoxLayout;
    button_hide = new QPushButton;
    button_delete = new QPushButton;
    button_compare = new QPushButton;
    buttons_middleLabel = new QLabel;

    sort = new QSortFilterProxyModel ;

    sort->setSortRole(Qt::UserRole);
    table->setModel(sort);

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //table.setItemDelegate(0);
    table->setSortingEnabled(1);

    connect(table,SIGNAL(pressed(QModelIndex)),this,SLOT(clicked(QModelIndex)));

    button_delete->setText("Delete");
    button_hide->setText("Hide");
    button_compare->setText("Coprare difference");
    buttons->addWidget(button_delete,0);
    buttons->addWidget(button_hide,0);
    buttons->addWidget(buttons_middleLabel,1);
    buttons->addWidget(button_compare,0);


    l->addWidget(table);
    l->addLayout(buttons);
    setLayout(l);

}
void PageViewer::setModel(QAbstractItemModel *model)
{
    sort->setSourceModel(model);
}
void PageViewer::clicked(QModelIndex index)
{
    qDebug ( ) << "clicked" << index.column() <<" "<< index.row();
    if(index.column()== 1)
    {
        //qDebug() << QUrl(QString("file:")+QDir::currentPath()+"/history/"+sort->data(index,Qt::DisplayRole).toString());
        QDesktopServices::openUrl(QUrl(QString("file:")+QDir::currentPath()+"/history/"+sort->data(index,Qt::DisplayRole).toString()));
    }
    if(index.column()==sort->columnCount()-1)
    {
        sort->removeRows(index.row(),1);
    }

}
