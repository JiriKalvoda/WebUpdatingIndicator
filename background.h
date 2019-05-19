#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtSql>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QLockFile>
#include <pagequery.h>
#include "conectionthread.h"
#include "monitoredvar.h"
#include "newpagemodel.h"
class Background : public QObject
{
    Q_OBJECT
private:
    bool isOkStart;
public:
    explicit Background(QObject *parent = 0);
    virtual ~Background();
    void inport(QString file);
    MonitoredVar bar;
    MonitoredVar console;
    ConectionThread * conTh = 0;
    NewPageModel newPages;
    QSqlDatabase db;
    QTimer startTimer;
    MonitoredVar barColor;
    MonitoredVar timeLastOk;
    MonitoredVar timeNextRun;
    MonitoredVar actPeriod;
    MonitoredVar timeStartLastAct;

    QVector <pageListItem> pageList;

    QLockFile * lockFileLock;

    bool errorStarting(){return !isOkStart;}

    PageQuery * history(QString query);
signals:
    void actualizacionColsole();
    void actualizationBar();
    void pageChanged_signal(NewPageItem);
public slots:
    void actualization();
    void stopActualization();
    void insertInConsole(QString in);
    void setBar(int in);
    void pageChanged(QString,QString);
    void errorCon(QString);
    void endOfRun(int errors,int ok);
    void changeActPeriod(int in);
    void actDbSetings();
    void inportDbSetings();
};



#endif // BACKGROUND_H
