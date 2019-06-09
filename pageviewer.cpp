#include "pageviewer.h"
#include <QHeaderView>
#include <debug.h>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QModelIndexList>
#include <newpagemodel.h>
#include <background.h>
#include <qmessagebox.h>
#include <QFile>

PageViewer::PageViewer(Background *bg, QWidget *parent) : QWidget(parent),bg(bg)
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

    connect(button_delete,SIGNAL(clicked(bool)),this,SLOT(deleteSlot()));
    connect(button_hide,SIGNAL(clicked(bool)),this,SLOT(hideSlot()));
    connect(button_compare,SIGNAL(clicked(bool)),this,SLOT(compareSlot()));

    l->addWidget(table);
    l->addLayout(buttons);
    setLayout(l);

}
void PageViewer::setModel(NewPageModel *model)
{
    inputModel=model;
    sort->setSourceModel(model);
}
void PageViewer::clicked(QModelIndex index)
{
    D_PAGEVIEWER qDebug ( ) << "clicked" << index.column() <<" "<< index.row();
    if(index.column()== 1)
    {
        //D_PAGEVIEWER qDebug() << QUrl(QString("file:")+QDir::currentPath()+"/history/"+sort->data(index,Qt::DisplayRole).toString());
        QDesktopServices::openUrl(QUrl(QString("file:")+QDir::currentPath()+"/history/"+sort->data(index,Qt::DisplayRole).toString()));
    }
    if(index.column()==sort->columnCount()-1)
    {
        sort->removeRows(index.row(),1);
    }

}

void PageViewer::hideSlot()
{
    QModelIndexList list = table->selectionModel()->selectedIndexes();
    QSet<int> selectedRows;
    for(auto i=list.begin();i!=list.end();i++)
        selectedRows.insert(sort->data(*i,NewPageModel::IdRole).toInt());
    for(auto i=selectedRows.begin();i!=selectedRows.end();i++)
        inputModel->hideHistPage(*i);
    needActualization();
}
void PageViewer::deleteSlot()
{
    QModelIndexList list = table->selectionModel()->selectedIndexes();
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Delete selected", QString("Are you realy want do delete ")+QString::number(list.size())+" selected record permanently (with history file)?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QSet<int> selectedRows;
        for(auto i=list.begin();i!=list.end();i++)
            selectedRows.insert(sort->data(*i,NewPageModel::IdRole).toInt());
        for(auto i=selectedRows.begin();i!=selectedRows.end();i++)
            inputModel->deleteHistPage(*i);
        needActualization();
    }
}
void PageViewer::compareSlot()
{
    QModelIndexList list = table->selectionModel()->selectedIndexes();
    QSet<int> selectedRows;
    for(auto i=list.begin();i!=list.end();i++)
        selectedRows.insert(sort->data(*i,NewPageModel::IdRole).toInt());
    if(selectedRows.size()==2)
    {
        auto a = selectedRows.begin();
        auto b = ++selectedRows.begin();
        (new PageComparatorGui(*a,*b,bg))->show();
    }
    if(selectedRows.size()==1)
    {
        auto a = selectedRows.begin();
        (new PageComparatorGui(*a,-1,bg))->show();
    }
}
