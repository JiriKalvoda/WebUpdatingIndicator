#include "background.h"
#include <debug.h>
#include <qFile>

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QObject>
#include <QNetworkCookieJar>
#include <QNetworkCookie>



Background::Background(QObject *parent) : QObject(parent),newPages(this)
{
    isOkStart=0;
    lockFileLock = new QLockFile("runlock.dat");
    if(!lockFileLock->tryLock(10)) return;
    isOkStart=1;
    //lockFileLock->lock();

    db = new Database;
    auto in = db->selectFromNewPage("where del = 0");
    for(auto & it:in)
            newPages.insert(it);

    inport("pages.json");
    inportDbSetings();

    conTh = new ConectionThread();
    connect(conTh,SIGNAL(textStatus(QString)),this,SLOT(insertInConsole(QString)));
    connect(conTh,SIGNAL(intStatus(int)),this,SLOT(setBar(int)));
    connect(conTh,SIGNAL(pageChanged(QString,QString)),this,SLOT(pageChanged(QString,QString)));
    connect(conTh,SIGNAL(error(QString)),this,SLOT(errorCon(QString)));
    connect(conTh,SIGNAL(endOfRun(int,int)),this,SLOT(endOfRun(int,int)));


    connect(&startTimer,SIGNAL(timeout()),this,SLOT(actualization()));
    startTimer.setSingleShot(1);

    connect(&timeLastOk,SIGNAL(change()),this,SLOT(actDbSetings()));
    connect(&timeNextRun,SIGNAL(change()),this,SLOT(actDbSetings()));
    connect(&actPeriod,SIGNAL(change()),this,SLOT(actDbSetings()));
    connect(&timeStartLastAct,SIGNAL(change()),this,SLOT(actDbSetings()));
}
Background::~Background()
{
    lockFileLock->unlock();
}


void Background::actDbSetings()
{
    if(!isOkStart) return;
    db->query( QString("update settings set ")// (timeLastOk,timeNextRun,actPeriod,timeStartLastAct) ")
            +"timeLastOk = '"+timeLastOk->toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
            +"', actPeriod = "+QString::number(actPeriod->toInt())
            +" , timeStartLastAct = '"+timeStartLastAct->toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
            +"'");
}

void Background::inportDbSetings()
{
    if(!isOkStart) return;
    QSqlQuery dotaz = db->query("select * from settings");
    if(dotaz.next())
    {
        timeLastOk = dotaz.value("timeLastOk");
        timeStartLastAct = dotaz.value("timeStartLastAct");
        changeActPeriod( dotaz.value("actPeriod").toInt() );
    }
    else
    {
        db->query("insert into settings (actPeriod) values (0)");
    }
}

void Background::inport(QString file_name)
{
    if(!isOkStart) return;
    QFile file(file_name);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    auto file_data = file.readAll();
    QJsonParseError err;
    QJsonDocument j = QJsonDocument::fromJson(file_data,&err);
    file.close();
    //D_JSON qDebug() <<err.errorString() ;
    //D_JSON qDebug() << j.isNull() << j.isObject() << j.isEmpty();
    if(j.isArray())
    {
        auto a = j.array();
        for(int i=0;i<a.size();i++)
        {
            if(a[i].isObject())
            {
                auto obj = a[i].toObject();
                pageListItem act;
                if(obj.value("server"      )!= QJsonValue::Undefined) act.url_server= obj["server"].toString();
                if(obj.value("dir"         )!= QJsonValue::Undefined) act.url_dir   = obj["dir"].toString();
                if(obj.value("file"        )!= QJsonValue::Undefined) act.url_file  = obj["file"].toString();
                if(obj.value("post"        )!= QJsonValue::Undefined) act.post      = obj["post"].toString();
                if(obj.value("name"        )!= QJsonValue::Undefined) act.name      = obj["name"].toString();
                if(obj.value("cookie"      )!= QJsonValue::Undefined) act.cookie    = obj["cookie"].toString();
                if(obj.value("diff"        )!= QJsonValue::Undefined) act.diff      = obj["diff"];
                pageList.push_back(act);
            }
            else D_JSON qDebug() << "------------- JSON ITEM ERROR";
        }
    }
    else D_JSON qDebug() << "------------- JSON ERROR";
}
void Background::actualization()
{
    if(!isOkStart) return;
    barColor="green";
    if(!conTh->isRunning())
    {
        conTh->stop=0;
        conTh->init(pageList);
        D_THRED qDebug() << "Strt Thred";
        conTh->start();
        timeStartLastAct = QDateTime::currentDateTime();
        if(actPeriod->toInt())
        {
            startTimer.start(actPeriod->toInt() * 60 * 1000);
            timeNextRun = QDateTime::currentDateTime().addMSecs(actPeriod->toInt()*60*1000);
        }
        else
        {
            startTimer.stop();
            timeNextRun=0;
        }
    }
    else
    {
        conTh->stop=1;
        startTimer.start(100);
        timeNextRun = QDateTime::currentDateTime().addMSecs(100);
    }
}
void Background::stopActualization()
{
    if(!isOkStart) return;
    conTh->stop=1;
}

void Background::insertInConsole(QString in)
{
    if(!isOkStart) return;
    console = QVariant(console->toString() + in);
}
void Background::setBar(int in)
{
    if(!isOkStart) return;
    bar = QVariant(in);
}
void Background::pageChanged(QString name,QString fileName)
{
    if(!isOkStart) return;
    auto it = NewPageItem{name,QDateTime::currentDateTime(),fileName,0,-1};
    db->insertIntoNewPage(it);
    QSqlQuery dotaz = db->query("select max(id) from newPage");
    if(dotaz.next()) it.id = dotaz.value(0).toInt();
    pageChanged_signal(it);
    newPages.insert(it);
}

void Background::errorCon(QString)
{
    if(!isOkStart) return;
    barColor="red";
}

void Background::endOfRun(int errors, int)
{
    if(!isOkStart) return;
    if(!errors)
    {
        timeLastOk = QDateTime::currentDateTime();
        D_THRED qDebug()<<"set timeLastOk";
    }
}
void Background::changeActPeriod(int in)
{
    if(!isOkStart) return;
    if(in>24*60*7) in = 24*60*7;
    actPeriod=in;
    if(in)
    {
        int toNextRun = in*60*1000 - timeStartLastAct->toDateTime().msecsTo(QDateTime::currentDateTime());
        if(toNextRun<=0) toNextRun = 100;
        if(timeStartLastAct->toDateTime().addDays(7)<=QDateTime::currentDateTime()) toNextRun = 100;
        startTimer.start(toNextRun);
        timeNextRun = QDateTime::currentDateTime().addMSecs(toNextRun);
    }
    else
    {
        startTimer.stop();
        timeNextRun=0;
    }
}
PageQuery * Background::history(QString query)
{
    auto out = new PageQuery(this);
    auto in = db->selectFromNewPage(query);
    for(auto & it:in)
            out->insert(it);
    return out;
}

