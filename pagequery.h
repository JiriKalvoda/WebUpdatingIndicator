#ifndef PAGEQUERY_H
#define PAGEQUERY_H

#include<newpagemodel.h>

#include <QObject>
class PageQuery : public NewPageModel
{
    Q_OBJECT

public:
    PageQuery();
    QString error;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

};

#endif // PAGEQUERY_H
