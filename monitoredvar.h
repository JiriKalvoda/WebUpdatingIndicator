#ifndef MONITOREDVAR_H
#define MONITOREDVAR_H

#include <QObject>
#include <QVariant>


class MonitoredVar : public QObject
{
    Q_OBJECT
public:
    explicit MonitoredVar(QObject *parent = 0):QObject (parent)
    {

    }
    explicit MonitoredVar(QVariant in,QObject *parent = 0):QObject (parent)
    {
        val=in;
        change();
    }
private:
    QVariant val;
signals:
    void change();
public:
    operator QVariant ()
    {
        return val;
    }
    QVariant operator = (QVariant in)
    {
        val = in;
        change();
        return *this;
    }
    const QVariant * operator -> ()
    {
        return &val;
    }
public slots:
};

#endif // MONITOREDVAR_H
