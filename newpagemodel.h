#ifndef NEWPAGEMODEL_H
#define NEWPAGEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <QDateTime>

class Background;
struct NewPageItem
{
    QString pageName;
    QDateTime time;
    QString fileName;
    bool del;
    int id;
    bool operator < (NewPageItem const & dr){return id<dr.id;}
};

class NewPageModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    Background * bg;
    static const int IdRole = Qt::UserRole+10;

    QVector<NewPageItem> v;
    explicit NewPageModel(Background * bg,QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool insert(NewPageItem in);
    virtual void hideHistPage(int id);
    virtual void deleteHistPage(int id);
private:
};

#endif // NEWPAGEMODEL_H
