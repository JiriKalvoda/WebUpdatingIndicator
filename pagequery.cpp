#include "pagequery.h"
#include <QSqlQuery>
#include <QDebug>
#include <background.h>

PageQuery::PageQuery(Background * bg):NewPageModel(bg)
{

}

bool PageQuery::removeRows(int row, int count, const QModelIndex &)
{
    for(int i=row;i<row+count;i++)
    {
        NewPageItem & it  = v[i];
        bg->newPages.hideHistPage(it.id);
        it.del=1;
        dataChanged(index(i,0),index(i,columnCount()-1));
    }
    return 0;
}
void PageQuery::hideHistPage(int id)
{
    bg->newPages.hideHistPage(id);
}

void PageQuery::deleteHistPage(int id)
{
    bg->newPages.deleteHistPage(id);
}
