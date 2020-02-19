#include "connectionthread.h"

#include "background.h"
#include <QFile>
#include <debug.h>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkCookieJar>
#include <QNetworkCookie>


ConnectionThread::ConnectionThread()
{

}
void ConnectionThread::init(QVector <pageListItem> PageList)
{
    pageList=PageList;
}

void ConnectionThread::stopNextPage()
{
    stop = 1;
}
void replaceByteArr(QByteArray & a,QByteArray start,QByteArray end,int countOfEnd)
{
    while(a.indexOf(start)>=0)
    {
        int s = a.indexOf(start);
        int e = s+1;
        for(int i=0;i<countOfEnd;i++)
            e = e==-1 ? -1 : a.indexOf(end,e)+end.size();
        if(e==-1) e = a.size();
        a.replace(s,e-s,QByteArray(""));
    }
}
bool ConnectionThread::control(const QByteArray & a,const QByteArray & b,QJsonValue & diff,QString outputFile)
{
    auto x =diffIgnore(a,diff);
    auto y =diffIgnore(b,diff);
    if(outputFile!="")
    {
        QFile file("comp/diffIgnore-"+outputFile+".html");
        file.open(QIODevice::WriteOnly);
        file.write(y);
        file.close();
    }
    return x!=y;
}
QByteArray ConnectionThread::diffIgnore(QByteArray in, QJsonValue & diff)
{
    if(diff.isObject())
    {
        auto diffObj = diff.toObject();
        if(diffObj.value("ignore")!=QJsonValue::Undefined) return "";
        if(diffObj.value("ignoreSector").isArray())
        {
            auto arr = diffObj["ignoreSector"].toArray();
            for(int i=0;i<arr.size();i++)
            {
                QVector<int> charToDelete(in.size());
                if(arr[i].isObject())
                {
                    auto obj = arr[i].toObject();
                    if(!obj["start"].isString()) continue;
                    QString start = obj["start"].toString();
                    QString end = obj["end"].isString() ? obj["end"].toString() : "\n";
                    int countOfEnd = obj["countOfEnd"].isDouble() ? int(obj["countOfEnd"].toDouble()+0.2) : 1;
                    for(int s=0;;s++)
                    {
                        s = in.indexOf(start,s);
                        if(s==-1) break;
                        int e = s+1;
                        for(int i=0;i<countOfEnd;i++)
                            e = e==-1 ? -1 : in.indexOf(end,e)+end.size();
                        if(e==-1) e = in.size();
                        for(int i=s;i<e;i++)
                            charToDelete[i]=1;
                    }
                }
                QByteArray out;
                for(int i=0;i<in.size();i++)
                    if(!charToDelete[i])
                        out.push_back(in[i]);
                out.swap(in);
            }
        }
        if(diffObj.value("onlySector").isArray())
        {
            auto arr = diffObj["onlySector"].toArray();
            for(int i=0;i<arr.size();i++)
            {
                QVector<int> charToDelete(in.size());
                if(arr[i].isObject())
                {
                    auto obj = arr[i].toObject();
                    if(!obj["start"].isString()) continue;
                    QString start = obj["start"].toString();
                    QString end = obj["end"].isString() ? obj["end"].toString() : "\n";
                    int countOfEnd = obj["countOfEnd"].isDouble() ? int(obj["countOfEnd"].toDouble()+0.2) : 1;
                    for(int s=0;;s++)
                    {
                        s = in.indexOf(start,s);
                        if(s==-1) break;
                        int e = s+1;
                        for(int i=0;i<countOfEnd;i++)
                            e = e==-1 ? -1 : in.indexOf(end,e)+end.size();
                        if(e==-1) e = in.size();
                        for(int i=s;i<e;i++)
                            charToDelete[i]=1;
                    }
                }
                QByteArray out;
                for(int i=0;i<in.size();i++)
                    if(charToDelete[i])
                        out.push_back(in[i]);
                out.swap(in);
            }
        }
        if(diffObj.value("permutation")!=QJsonValue::Undefined)
        {
            const int countOfChar = 260;
            int c[countOfChar];
            for(int i=0;i<countOfChar;i++) c[i]=0;
            for(int i=0;i<in.size();i++) c[(unsigned char) (in[i])]++;
            in.clear();
            for(int i=0;i<countOfChar;i++)
                in.push_back((QString::number(c[i])+"\n").toUtf8());
        }
    }
    if(diff.isString()&&diff.toString()=="ignore") return "";
    return in;

}
void ConnectionThread::run()
{
    QNetworkAccessManager manager;
    QMap <QString , QNetworkCookieJar *> cookieJar;
    QMap <QString, bool> errorWhitCookieLine;
    int page_errors=0,pages_ok=0,pages_act=0;
    for(int i=0;i<pageList.size();i++)
    {
        if(stop)
        {
            textStatus("\n********STOP!********\n");
            exit(0);
            return;
        }
        auto act = &(pageList[i]);
        intStatus(100*i/pageList.size());
        QUrl url (act->url());
        QString name = act->name_func();
        textStatus(QString("\n******************** ")+name);
        if(act->cookie!="")
        {
            if(errorWhitCookieLine[act->cookie])
            {
                page_errors++;
                textStatus("\n!!!!!!!!!!!!!!!!!!! Cookie error - cookie not available");
                error("Cookie not avaieble");
                continue;
            }
            if(!cookieJar[act->cookie])
                cookieJar[act->cookie] = new QNetworkCookieJar (&manager);
            manager.setCookieJar(cookieJar[act->cookie]);
        }
        else
        {
            manager.setCookieJar(new QNetworkCookieJar (&manager));
        }
        QByteArray data;

        QNetworkRequest request(url);
        QNetworkReply * reply;
        if(act->post!="")
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
            reply= manager.post(request,act->post.toUtf8());
        }
        else
        {
            reply=manager.get(request);
        }
        QEventLoop loop;
        //----------------------------------------------
        QTimer timer;
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        timer.start(10*1000);   // 30 secs. timeout
        loop.exec();

        if(!timer.isActive())
        {
           // timeout
           disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
           reply->abort();
           page_errors++;
           textStatus("\n!!!!!!!!!!!!!!!!!!! Connection error - TIME OUT");
           error("TIME OUT");
           errorWhitCookieLine[act->cookie]=1;
           continue;

        }
        //----------------------------------------------
        data = reply->readAll();
                {
                    auto rhplist = reply->rawHeaderPairs();
                    auto rhpi = rhplist.constBegin() ;
                    for (;rhpi!=rhplist.constEnd(); rhpi++) {
                      if ((rhpi->first=="Content-Encoding")) {
                          textStatus(QString()+" ["+rhpi->second+"]");
                      }
                    }
                }

        {
            QString root = act->url_server;
            QString dir = act->url_serverDir()+"/";
            replaceWebHrefToAbsolute(data,dir,root);
        }
        if(reply->error())
        {
            error(reply->errorString());
            errorWhitCookieLine[act->cookie]=1;
            page_errors++;
            textStatus("\n!!!!!!!!!!!!!!!!!!! Connection error - "+reply->errorString());
            error(reply->errorString());
        }
        else
        {
            pages_ok++;
            //textStatus("\tlen: "+QString::number(data.size()));
            QFile fileR("pages/"+name+".html");
            QByteArray fileData = "";
            if(fileR.open(QIODevice::ReadOnly))
            {
                fileData = fileR.readAll();
                fileR.close();
            }
            if(control(fileData,data,act->diff,debugDiff?act->name_func():""))
            {
                QFile fileW("pages/"+name+".html");
                fileW.open(QIODevice::WriteOnly);
                fileW.write(data);
                fileW.close();
                auto time = QDateTime ::currentDateTime();
                QString stime = time.toString("yy-MM-dd-h-m-s");
                QString hName = name+"--"+stime+".html";
                QFile fileH("history/"+hName);
                fileH.open(QIODevice::WriteOnly);
                fileH.write(data);
                fileH.close();
                textStatus("\n--------------------------- page changed");
                pages_act++;
                pageChanged(name,hName);
            }
        }
    }
    intStatus(100);
    textStatus("\n*********END********* ("+QString::number(pages_act)+" change)\n");
    endOfRun(page_errors,pages_ok);
    exit(0);
}

inline void replaceWebHrefToAbsoluteSpecPar(QByteArray & data,QString & dir,QString & root,QString tag,QString uv)
{

    data.replace((tag+"="+uv+"/").toUtf8(),
                (tag+"="+uv+root+"/").toUtf8());
    data.replace((tag+"="+uv+"http").toUtf8(),
                 (tag+"_http"+uv).toUtf8());
    data.replace((tag+"="+uv+"//").toUtf8(),
                 (tag+"_ll"+uv).toUtf8());
    data.replace((tag+"="+uv).toUtf8(),
                (tag+"="+uv+dir).toUtf8());
    data.replace((tag+"_http"+uv).toUtf8(),
                 (tag+"="+uv+"http").toUtf8());
    data.replace((tag+"_ll"+uv).toUtf8(),
                 (tag+"="+uv+"http").toUtf8());
}

void replaceWebHrefToAbsolute(QByteArray & data,QString & dir,QString & root)
{
    replaceWebHrefToAbsoluteSpecPar(data,dir,root,"href","'" );
    replaceWebHrefToAbsoluteSpecPar(data,dir,root,"href","\"");
    replaceWebHrefToAbsoluteSpecPar(data,dir,root,"src" ,"'" );
    replaceWebHrefToAbsoluteSpecPar(data,dir,root,"src" ,"\"");
}

