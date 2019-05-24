#ifndef PAGEQUERY_H
#define PAGEQUERY_H

#include<newpagemodel.h>

#include <QObject>
class PageQuery : public NewPageModel
{
    Q_OBJECT

public:
    PageQuery(Background *bg);
    QString error;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual void hideHistPage(int id);
    virtual void deleteHistPage(int id);
};

#endif // PAGEQUERY_H
