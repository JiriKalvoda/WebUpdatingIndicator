#include "pagecomparator.h"
#include <QFile>
#include <QDesktopServices>
#include <debug.h>
#include <database.h>
#include <newpagemodel.h>


PageComparator::PageComparator(int a,int b,Background * Bg,QObject *parent):
    QObject(parent),bg(Bg)
{
    setNewPrefix();
    calculatePages(a,b);
}
PageComparator::~PageComparator()
{
    D_PAGECOMPARATOR qDebug() << "~PageComparator";
    for(auto it=openFile.begin();it!=openFile.end();it++)
    {
        QFile file (*it);
        file.remove();
        D_PAGECOMPARATOR qDebug()<<"remove file "<<*it;
    }
}

void PageComparator::setNewPrefix()
{
    static int c = 0;
    filePrefix=QString("")+QString::number(c++)+"-";
    fileDirPrefix=QString("comp/")+filePrefix;
}
void PageComparator::generateHeadHtml(char ab,NewPageItem  npit)
{
    QFile file (fileDirPrefix+ab+"Name.html");
    openFile.insert(fileDirPrefix+ab+"Name.html");
    file.open( QIODevice::WriteOnly);
    file.write(("<!DOCTYPE HTML>\n<HTML>\n<head>\n"
                "<body style=\"margin:0pt;padding:0pt\">"
                "<h1 align=\"center\" style=\"margin:0pt;padding:3pt\">"+
                     npit.pageName+" "+npit.time.toString("d.M.yy h:mm:ss")+
                     "</h1></body></head>\n").toUtf8());
    file.close();
}

void PageComparator::generate(bool viewSC)
{
    pageWriteiterator++;
    QString body = QString("")+
            "<div id=\"a\" style=\"position: absolute;left: 0%;width:50%;top: 0px;height: 100%;\">"
            "<h1 style=\"text-align: center\">"
            +page_a.pageName+" "+page_a.time.toString("d.M.yy h:mm:ss")+
            "</h1>"
            "<iframe src=\""+filePrefix+"a.html\" id=\"a-page\" "
                    "style=\" position: absolute;left: 0px;width:100%;top: 70px;height: calc(100% - 70px );\">"
            "</iframe>"
            "</div>"
            "<div id=\"b\" style=\"position: absolute;left: 50%;width:50%;top: 0px;height: 100%;\">"
            "<h1 style=\"text-align: center\">"
            +page_b.pageName+" "+page_b.time.toString("d.M.yy h:mm:ss")+
            "</h1>"
            "<iframe src=\""+filePrefix+"b.html\" id=\"b-page\" "
                  "style=\" position: absolute;left: 0px;width:100%;top: 70px;height: calc(100% - 70px );\"></iframe>"
            "</div>";
    QString document = QString("")+
             "<!DOCTYPE HTML>"
             "<HTML>"
             "<head>"
             "<title>WebUpdatingIndicator - Copmepre page</title>"
             "<meta http-equiv=\"Content-Type\">"
             "<meta http-equiv=\"Cache-control\" content=\"no-cache\">"
             "<meta http-equiv=\"Pragma\" content=\"no-cache\">"
             "<meta http-equiv=\"expires\" content=\"0\">"
             "<script>"
             "function removeActSrc() {"
             "    var element = document.getElementById(\"actScript\");"
             "    if(element!=null) element.parentNode.removeChild(element);"
             "}"
             "function nacistJs(url) {"
             "  removeActSrc();"
             "  var script = document.createElement(\"script\");"
             "  script.src = url;"
             "  script.id=\"actScript\";"
             "  document.getElementsByTagName('head')[0].appendChild(script);"
             "};"
             "function actFunction(){"
             "  nacistJs(\""+filePrefix+"act.js\");"
             " setTimeout(actFunction, 100);"
             "};"
             "actFunction();"
             "var oldToken = "+QString::number(pageWriteiterator)+";"
             "function ifnewact(token)"
             "{"
             "  if(token!=oldToken) location.reload(1);"
             "}"
             "</script>"
             "</head>"
             "<body>"
             +body+
             "</body>"
             "</html>";
    QFile fileMain (fileDirPrefix+"main.html");
    openFile.insert(fileDirPrefix+"main.html");
    fileMain.open( QIODevice::WriteOnly);
    fileMain.write((document).toUtf8());
    fileMain.close();
    QFile fileA (fileDirPrefix+"a.html");
    openFile.insert(fileDirPrefix+"a.html");
    fileA.open( QIODevice::WriteOnly);
    fileA.write(generateIframe(0,viewSC).toUtf8());
    fileA.close();
    QFile fileB (fileDirPrefix+"b.html");
    openFile.insert(fileDirPrefix+"b.html");
    fileB.open(QIODevice::WriteOnly);
    fileB.write(generateIframe(1,viewSC).toUtf8());
    fileB.close();
    QFile fileAct  (fileDirPrefix+"act.js");
    openFile.insert(fileDirPrefix+"act.js");
    fileAct.open(QIODevice::WriteOnly);
    fileAct.write(QString(QString("")+"ifnewact("+QString::number(pageWriteiterator)+")").toUtf8());
    fileAct.close();
}
QString PageComparator::generateIframe(int id, bool viewSC)
{
    QString out;
    QString beforeDiff = "<span style=\"background-color:red\">";
    QString afterDiff =  "</span>";
    for(int i=0;i<std::min(data.size(),dataFile.size());i++)
    {
        if(data[i][0] == '<')
        {
            if(dataFile[i]&(1<<id)) out+=data[i];
        }
        else
        {
            if(dataFile[i]&(1<<id)) out+=(dataFile[i]==3?"":beforeDiff)+data[i].replace("\n","")+(dataFile[i]==3?"":afterDiff)+"\n";
        }
    }
    return out;
}
void PageComparator::open()
{
    QDesktopServices::openUrl(QUrl(QString("file:")+QDir::currentPath()+"/"+fileDirPrefix+"main.html"));
}
void PageComparator::calculatePages(int a,int b)
{ 
    page_a = page_b = NewPageItem();
    id_a=a;
    auto tmp_a = bg->db->selectFromNewPage("where id = "+QString::number(id_a));
    if(tmp_a.size()) page_a = tmp_a[0];
    id_b=b;
    auto tmp_b = id_b==-1 ?
               bg->db->selectFromNewPage(
                    "where pageName = '"+page_a.pageName+
                    "' and time < '"+page_a.time.toString("yyyy-MM-dd hh:mm:ss.zzz") +
                    "' ORDER BY time DESC LIMIT 1")
              :bg->db->selectFromNewPage("where id = "+QString::number(id_b));
    if(tmp_b.size()) page_b = tmp_b[0];
    load();
}
void PageComparator::load()
{
    data.clear();
    dataFile.clear();
    auto a = loadFile(page_a);
    auto b = loadFile(page_b);
    int * * strDiffLen = new int * [a.size()];
    strDiffLen[0] = new int [a.size()*b.size()];
    D_PAGECOMPARATOR qDebug()<<"PageComparator load init arr ok";
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
    D_PAGECOMPARATOR qDebug()<<"PageComparator load calc arr ok";
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
        D_PAGECOMPARATOR qDebug()<<"ERR PageComparator load init make" <<i<<" "<<j<<" from "<<a.size()<<" "<<b.size();
    }
   D_PAGECOMPARATOR  qDebug()<<"PageComparator load init make ok";
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

QStringList PageComparator::loadFile(NewPageItem in)
{
    if(in.fileName!="")
    {
        D_PAGECOMPARATOR qDebug() << "LOAD: " << "history/"+in.fileName;
        QFile file("history/"+in.fileName);
        file.open(QIODevice::ReadOnly);
        auto dat =QString::fromUtf8(file.readAll());
        file.close();
        return parseData(dat);
    }
    return QStringList()<<"";
}
