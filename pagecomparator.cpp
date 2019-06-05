#include "pagecomparator.h"
#include <QFile>
#include <QDesktopServices>
#include <qdebug.h>


PageComparator::PageComparator(int a,int b,Background * Bg,QObject *parent):
    QObject(parent),bg(Bg),id_a(a),id_b(b)
{
    setNewPrefix();
    load();
}
PageComparator::~PageComparator()
{
    qDebug() << "~PageComparator";
    for(auto it=openFile.begin();it!=openFile.end();it++)
    {
        QFile file (*it);
        file.remove();
        qDebug()<<"remove file "<<*it;
    }
}

void PageComparator::setNewPrefix()
{
    static int c = 0;
    filePrefix=QString("")+QString::number(c++)+"-";
    fileDirPrefix=QString("comp/")+filePrefix;
}

void PageComparator::generate()
{
    QFile fileMain (fileDirPrefix+"main.html");
    openFile.insert(fileDirPrefix+"main.html");
    fileMain.open( QIODevice::WriteOnly);
    fileMain.write(
                (QString("")+
                        "<!DOCTYPE HTML>\n"
                        "<HTML>\n"
                        "<head>\n"
                        "<title>WebUpdatingIndicator - Copmepre page</title>\n"
                        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1250\">\n"
                        "<meta http-equiv=\"Cache-control\" content=\"no-cache\">\n"
                        "<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"
                        "<meta http-equiv=\"expires\" content=\"0\">\n"
                        "</head>\n"
                        "<frameset  rows=\"75,*\" frameborder=\"0\" framespacing=\"0\">\n"
                        "<frameset  cols=\"*,*\" frameborder=\"0\" framespacing=\"0\">\n"
                        "<frame name=\"page1\" src=\""+filePrefix+"aName.html"+"\" marginwidth=\"0\" marginheight=\"5\" scrolling=\"auto\" frameborder=\"no\" noresize>\n"
                        "<frame name=\"page2\" src=\""+filePrefix+"bName.html"+"\" marginwidth=\"0\" marginheight=\"5\" scrolling=\"auto\" frameborder=\"no\" noresize>\n"
                        "</frameset>\n"
                        "<frameset  cols=\"*,*\" frameborder=\"0\" framespacing=\"0\">\n"
                        "<frame name=\"page1\" src=\""+filePrefix+"a.html"+"\" marginwidth=\"0\" marginheight=\"5\" scrolling=\"auto\" frameborder=\"no\" noresize>\n"
                        "<frame name=\"page2\" src=\""+filePrefix+"b.html"+"\" marginwidth=\"0\" marginheight=\"5\" scrolling=\"auto\" frameborder=\"no\" noresize>\n"
                        "</frameset>\n"
                        "</frameset>\n"
                        "</html>\n"
                        ).toUtf8());
    fileMain.close();
    QString data_fileA,data_fileB;
    QString beforeDiff = "<span style=\"background-color:red\">";
    QString afterDiff =  "</span>";
    for(int i=0;i<std::min(data.size(),dataFile.size());i++)
    {
        if(data[i][0] == '<')
        {
            if(dataFile[i]&(1<<0)) data_fileA+=data[i].replace("\n","")+"\n";
            if(dataFile[i]&(1<<1)) data_fileB+=data[i].replace("\n","")+"\n";
        }
        else
        {
            if(dataFile[i]&(1<<0)) data_fileA+=(dataFile[i]==3?"":beforeDiff)+data[i].replace("\n","")+(dataFile[i]==3?"":afterDiff)+"\n";
            if(dataFile[i]&(1<<1)) data_fileB+=(dataFile[i]==3?"":beforeDiff)+data[i].replace("\n","")+(dataFile[i]==3?"":afterDiff)+"\n";
        }
    }
    QFile fileA (fileDirPrefix+"a.html");
    openFile.insert(fileDirPrefix+"a.html");
    fileA.open( QIODevice::WriteOnly);
    fileA.write(data_fileA.toUtf8());
    fileA.close();
    QFile fileB (fileDirPrefix+"b.html");
    openFile.insert(fileDirPrefix+"b.html");
    fileB.open(QIODevice::WriteOnly);
    fileB.write(data_fileB.toUtf8());
    fileB.close();
    QFile fileAName (fileDirPrefix+"aName.html");
    openFile.insert(fileDirPrefix+"aName.html");
    fileAName.open( QIODevice::WriteOnly);
    fileAName.write(("<!DOCTYPE HTML>\n<HTML>\n<head>\n<body><h1 align=\"center\">\t"+QString::number(id_a)+"</h1></body></head>\n").toUtf8());
    fileAName.close();
    QFile fileBName (fileDirPrefix+"bName.html");
    openFile.insert(fileDirPrefix+"bName.html");
    fileBName.open(QIODevice::WriteOnly);
    fileBName.write(("<!DOCTYPE HTML>\n<HTML>\n<head>\n<body><h1 align=\"center\">"+QString::number(id_b)+"</h1></body></head>\n").toUtf8());
    fileBName.close();
}
void PageComparator::open()
{
    QDesktopServices::openUrl(QUrl(QString("file:")+QDir::currentPath()+"/"+fileDirPrefix+"main.html"));
}
void PageComparator::calculatePages(int a,int b)
{
    id_a=a;
    id_b=b;
    load();

}
void PageComparator::load()
{
    data.clear();
    dataFile.clear();
    auto a = loadFile(id_a);
    auto b = loadFile(id_b);
    int * * strDiffLen = new int * [a.size()];
    strDiffLen[0] = new int [a.size()*b.size()];
    qDebug()<<"PageComparator load init arr ok";
    for(int i=1;i<a.size();i++) strDiffLen[i]=strDiffLen[i-1]+b.size();
    for(int i=a.size()-1;i>=0;i--)
    {
        for(int j=b.size()-1;j>=0;j--)
        {
            int act = 1<<30;
            if(i+1<a.size()) act = std::min(act,strDiffLen[i+1][j]+1);
            if(j+1<b.size())act = std::min(act,strDiffLen[i][j+1]+1);
            if(i+1<a.size()&&j+1<b.size()) if(a[i+1]==b[j+1]) act = std::min(act,strDiffLen[i+1][j+1]);
            if(i+1==a.size()&&j+1==b.size()) act = 0;
            strDiffLen[i][j]=act;
        }
    }
    qDebug()<<"PageComparator load calc arr ok";
    for(int i=0,j=0;;)
    {
        int act = strDiffLen[i][j];
        if(i+1<a.size()&&j+1<b.size()) if(a[i+1]==b[j+1]) if(act == strDiffLen[i+1][j+1])
        {
            i++;j++;
           data << a[i];
           dataFile << ((1<<1)|(1<<0));
           continue;
        }
        if(i+1<a.size()) if(act == strDiffLen[i+1][j]+1)
        {
            i++;
           data << a[i];
           dataFile << ((1<<0));
           continue;
        }
        if(j+1<b.size()) if(act == strDiffLen[i][j+1]+1)
        {
           j++;
           data << b[j];
           dataFile << ((1<<1));
           continue;
        }
        if(i+1==a.size() && j+1==b.size())
        {
            break;
        }
        qDebug()<<"ERR PageComparator load init make" <<i<<" "<<j<<" from "<<a.size()<<" "<<b.size();
    }
    qDebug()<<"PageComparator load init make ok";
    delete [] (strDiffLen[0]);
    delete [] strDiffLen;
}
#define PUSH {if(!empty) {out << "";empty=1;}}
QStringList PageComparator::parseData(QString in)
{
    QStringList out = (QStringList()<< ""<<"");
    bool empty = 1;
    bool inTag=0;
//    bool lastSpace=0;
    for(int i=0;i<in.length();i++)
    {
        if(!inTag && (in[i].isSpace())) PUSH;
        if(in[i]=='<'&&!inTag) {PUSH;inTag=1;}

        out[out.size()-1]+=in[i];
        if(!in[i].isSpace()) empty=0;

        if(in[i]=='>'&&inTag) {PUSH;inTag=0;}

    }
    for(int i=0;i<out.length();i++)
    {
        out[i]=out[i].trimmed();
        if(i+1<out.length() && out[i+1][0].isSpace()) out[i]+=out[i+1][0];
    }
    return out;
}

QStringList PageComparator::loadFile(int id)
{
    QSqlQuery dotaz;
    dotaz.exec(QString("")+"select * from newPage where id = "+QString::number(id));
    if(dotaz.next())
    {
        NewPageItem in;
        in.pageName = dotaz.value("pageName").toString();
        in.time  = dotaz.value("time").toDateTime();
        in.fileName  = dotaz.value("fileName").toString();
        in.del = dotaz.value("del").toBool();
        in.id = dotaz.value("id").toInt();
        qDebug() << "LOAD: " << "history/"+in.fileName;
        QFile file("history/"+in.fileName);
        file.open(QIODevice::ReadOnly);
        auto dat =QString::fromUtf8(file.readAll());
        file.close();
        return parseData(dat);
    }
    return QStringList();
}
