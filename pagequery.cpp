#include "pagequery.h"
#include <QSqlQuery>
#include <QDebug>

PageQuery::PageQuery()
{

}

bool PageQuery::removeRows(int row, int count, const QModelIndex &parent)
{
    for(int i=row;i<row+count;i++)
    {
        NewPageItem & it  = v[i];
        QSqlQuery dotaz;
        QString q = QString("update newPage set del = 1 where ")
                +"pageName = '"+it.pageName
                +"' and time = '"+it.time.toString("yyyy-MM-dd hh:mm:ss.zzz")
                +"' and fileName = '"+it.fileName
                +"'";
        qDebug()<<q;
        dotaz.exec(q);
        it.del=1;
    }
    return 1;
}
