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
                        "<frameset  rows=\"*\" frameborder=\"0\" framespacing=\"0\">\n"
                        "<frameset  cols=\"*,*\" frameborder=\"0\" framespacing=\"0\">\n"
                        "<frame name=\"page1\" src=\""+filePrefix+"a.html"+"\" marginwidth=\"0\" marginheight=\"5\" scrolling=\"auto\" frameborder=\"no\" noresize>\n"
                        "<frame name=\"page2\" src=\""+filePrefix+"b.html"+"\" marginwidth=\"0\" marginheight=\"5\" scrolling=\"auto\" frameborder=\"no\" noresize>\n"
                        "</frameset>\n"
                        "</frameset>\n"
                        "</html>\n"
                        ).toUtf8());
    fileMain.close();
    QString data_fileA,data_fileB;
    for(int i=0;i<std::min(data.size(),dataFile.size());i++)
    {
        if(dataFile[i]&(1<<0)) data_fileA+=data[i];
        if(dataFile[i]&(1<<1)) data_fileB+=data[i];
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
    for(int i=0;i<a.size();i++)
    {
        data << a[i];
        dataFile << (1<<0);
    }
    for(int i=0;i<b.size();i++)
    {
        data << b[i];
        dataFile << (1<<1);
    }
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
        return QStringList () << dat;
    }
    return QStringList();
}
