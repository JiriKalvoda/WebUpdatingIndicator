#include "newpagemodel.h"
#include <qDebug>
#include <QSqlQuery>

NewPageModel::NewPageModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant NewPageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            if(section==0) return "Name";
            if(section==1) return "File";
            if(section==2) return "Time";
            if(section==3) return "Delete";
        }
    }
    else
        if(role == Qt::DisplayRole) return section+1;
    return QVariant();
}


int NewPageModel::rowCount(const QModelIndex &) const
{
    //if (!parent.isValid())
      //  return 0;
    return v.size();
    // FIXME: Implement me!
}

int NewPageModel::columnCount(const QModelIndex &) const
{
    //if (!parent.isValid())
      //  return 0;
    return 4;

    // FIXME: Implement me!
}

QVariant NewPageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(index.column()==0)
    {
        if(role == Qt::DisplayRole || role == Qt::UserRole) return QVariant(v[index.row()].pageName);
    }
    if(index.column()==1)
    {
        if(role == Qt::DisplayRole || role == Qt::UserRole) return QVariant(v[index.row()].fileName);
    }
    if(index.column()==2)
    {
        if(role == Qt::DisplayRole) return QVariant(v[index.row()].time.toString("d.M.yy h:mm:ss"));
        if(role == Qt::UserRole) return QVariant(v[index.row()].time.toTime_t());
    }
    if(index.column()==3)
    {
        if(role == Qt::DisplayRole || role == Qt::UserRole)
            return QVariant(v[index.row()].del?"":"DEL");
    }
    return QVariant();
}

bool NewPageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags NewPageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    if(v[index.row()].del)
        return Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable; // FIXME: Implement me!
}

bool NewPageModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return 0;
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}
bool NewPageModel::insert(NewPageItem in)
{
    beginInsertRows(QModelIndex(), 0, 0+1-1);
    // FIXME: Implement me!
    v.push_front(in);
    endInsertRows();
    qDebug() << "INSERTROWS";
    return 1;
}

bool NewPageModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
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
        }
        v.remove(row,count);
    endRemoveRows();
    return 1;
}
