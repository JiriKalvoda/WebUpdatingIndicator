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
#include <QEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QApplication>

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

    button_delete->setText("De&lete");
    button_hide->setText("H&ide");
    button_compare->setText("C&omprare difference");
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

    qApp->installEventFilter(this);

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
            QMessageBox::question(this, "Delete selected", QString("Are you really want do delete ")+QString::number(list.size())+" selected record permanently (with history file)?",
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
        auto cGui = (new PageComparatorGui(*a,*b,bg));
        cGui->show();
        cGui->showInGoodPlace();
    }
    if(selectedRows.size()==1)
    {
        auto a = selectedRows.begin();
        auto cGui = (new PageComparatorGui(*a,-1,bg));
        cGui->show();
        cGui->showInGoodPlace();
    }
}

bool PageViewer::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        int k = key->key();
        if(k==Qt::Key_Escape)
        {
            setFocus();
            numInput=0;
        }
        if(obj == this || obj == table)
        {
            if(Qt::Key_0<=k && k<=Qt::Key_9)
                numInput=numInput*10+k-Qt::Key_0;
            else if(k==Qt::Key_D)
            {
                for(int i=0;i<sort->rowCount();i++)
                    if(sort->headerData(i,Qt::Vertical)==QString::number(numInput))
                    {
                        table->pressed(sort->index(i,3));
                        break;
                    }
                numInput=0;
            }
            else if(k==Qt::Key_F)
            {
                for(int i=0;i<sort->rowCount();i++)
                    if(sort->headerData(i,Qt::Vertical)==QString::number(numInput))
                    {
                        table->pressed(sort->index(i,1));
                        break;
                    }
                numInput=0;
            }
            else if(k==Qt::Key_S)
            {
                for(int i=0;i<sort->rowCount();i++)
                    if(sort->headerData(i,Qt::Vertical)==QString::number(numInput))
                    {
                        table->selectRow(i);
                        break;
                    }
                numInput=0;
            }
            else if(k==Qt::Key_A)
            {
                for(int i=0;i<sort->rowCount();i++)
                    if(sort->headerData(i,Qt::Vertical)==QString::number(numInput))
                    {
                        auto lsmode = table->selectionMode();
                        table->setSelectionMode(QAbstractItemView::MultiSelection);
                        table->selectRow(i);
                        table->setSelectionMode(lsmode);
                        break;
                    }
                numInput=0;
            }
            else if(k==Qt::Key_D)
            {
                for(int i=0;i<sort->rowCount();i++)
                    if(sort->headerData(i,Qt::Vertical)==QString::number(numInput))
                    {
                        table->pressed(sort->index(i,3));
                        break;
                    }
                numInput=0;
            }
            else if(k==Qt::Key_L)
            {
                button_delete->click();
            }
            else if(k==Qt::Key_I)
                button_hide->click();
            else if(k==Qt::Key_O)
                button_compare->click();
            else if(k==Qt::Key_J)
                table->verticalScrollBar()->setValue(table->verticalScrollBar()->value()+table->verticalScrollBar()->singleStep()*3);
            else if(k==Qt::Key_K)
                table->verticalScrollBar()->setValue(table->verticalScrollBar()->value()-table->verticalScrollBar()->singleStep()*3);
            else if(k==Qt::Key_V && QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true)
                table->sortByColumn(0,Qt::SortOrder::DescendingOrder);
            else if(k==Qt::Key_B && QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true)
                table->sortByColumn(1,Qt::SortOrder::DescendingOrder);
            else if(k==Qt::Key_N && QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true)
                table->sortByColumn(2,Qt::SortOrder::DescendingOrder);
            else if(k==Qt::Key_M && QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true)
                table->sortByColumn(3,Qt::SortOrder::DescendingOrder);
            else if(k==Qt::Key_V)
                table->sortByColumn(0,Qt::SortOrder::AscendingOrder);
            else if(k==Qt::Key_B)
                table->sortByColumn(1,Qt::SortOrder::AscendingOrder);
            else if(k==Qt::Key_N)
                table->sortByColumn(2,Qt::SortOrder::AscendingOrder);
            else if(k==Qt::Key_M)
                table->sortByColumn(3,Qt::SortOrder::AscendingOrder);
            else
            {
                return QObject::eventFilter(obj, event);
            }
            return 1;
        }
    }
    return QObject::eventFilter(obj, event);
}
