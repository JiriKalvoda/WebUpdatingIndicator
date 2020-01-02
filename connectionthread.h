#ifndef CONECTIONTHREAD_H
#define CONECTIONTHREAD_H
#include <QThread>
#include <QObject>
#include <QVector>
#include <QString>
#include <QJsonValue>
class pageListItem //: public QObject
{

public:
    QString url_server;
    QString url_dir;
    QString url_file;
    QString post;
    QString name;
    QString cookie;
    QJsonValue diff;
    pageListItem(){}
    virtual ~pageListItem(){}
    QString url()
    {
       return    url_server +
               QString(url_dir!=""?"/":"")+url_dir +
               "/"+url_file;
    }
    QString url_serverDir()
    {
       return    url_server +
               (url_dir!=""?"/":"")+url_dir;
    }
    QString name_func()
    {
        if(name!="") return name;
        QString tmp = url();
        tmp.replace( QRegExp( "[" + QRegExp::escape( "\\/:*?\"<>|." ) + "]"),"-");
        return tmp;
    }
};

class ConnectionThread : public QThread
{
    Q_OBJECT
public:
    ConnectionThread();
    void init(QVector <pageListItem> PageList);
    virtual ~ConnectionThread(){}
    QVector <pageListItem> pageList;
    bool stop=0;
protected:
    void run();
signals:
    void textStatus(QString s);
    void intStatus(int out);
    void pageChanged(QString name,QString newFileName);
    void error(QString err);
    void endOfRun(int errors,int ok);
public slots:
    void stopNextPage();
};

void replaceWebHrefToAbsolute(QByteArray & data, QString &dir, QString &root);


#endif // CONECTIONTHREAD_H
