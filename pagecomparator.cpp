#include "pagecomparator.h"
#include <QFile>
#include <QDesktopServices>
#include <debug.h>
#include <database.h>
#include <newpagemodel.h>

bool substrComp(QString & s,int poz,QString comp)
{
    if(poz<0) return 0;
    if(poz+comp.size()>s.size()) return 0;
    for(int i=0;i<comp.size();i++)
        if(s[poz+i] != comp[i])
            return 0;
    return 1;
}

QString makeHtml(QString body, QString filePrefix, int pageWriteiterator)
{
    QString document = QString("")+
             "<!DOCTYPE HTML>\n"
             "<HTML>\n"
             "<head>\n"
             "<title>WebUpdatingIndicator - Copmepre page</title>\n"
             "<meta http-equiv=\"Content-Type\">\n"
             "<meta http-equiv=\"Cache-control\" content=\"no-cache\">\n"
             "<meta http-equiv=\"Pragma\" content=\"no-cache\">\n"
             "<meta http-equiv=\"expires\" content=\"0\">\n"
             "<script>\n"
             "function removeActSrc() {\n"
             "    var element = document.getElementById(\"actScript\");\n"
             "    if(element!=null) element.parentNode.removeChild(element);\n"
             "}\n"
             "function nacistJs(url) {\n"
             "  removeActSrc();\n"
             "  var script = document.createElement(\"script\");\n"
             "  script.src = url;\n"
             "  script.id=\"actScript\";\n"
             "  document.getElementsByTagName('head')[0].appendChild(script);\n"
             "};\n"
             "function actFunction(){\n"
             "  nacistJs(\""+filePrefix+"act.js\");\n"
             " setTimeout(actFunction, 100);\n"
             "};\n"
             "actFunction();\n"
             "var oldToken = "+QString::number(pageWriteiterator)+";\n"
             "function ifnewact(token)\n"
             "{\n"
             "  if(token!=oldToken) location.reload(1);\n"
             "}\n"
             "</script>\n"
             "</head>\n"
             "<body>\n"
             +body+"\n"
             "</body>\n"
             "</html>\n";
    return document;
}


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


void PageComparator::generate(int flags)
{
    pageWriteiterator++;
    QString body = QString("")+
            "<div id=\"a\" style=\"position: absolute;left: 0%;width: calc(50% - 2px );top: 0px;height: 100%;\">\n"
            "<h1 style=\"text-align: center\">\n"
            +page_a.pageName+" "+page_a.time.toString("d.M.yy h:mm:ss")+
            "</h1>\n"
            "</div>\n"
            "<div id=\"b\" style=\"position: absolute;left: 50%;width: calc(50% - 4px );top: 0px;height: 100%;\">\n"
            "<h1 style=\"text-align: center\">\n"
            +page_b.pageName+" "+page_b.time.toString("d.M.yy h:mm:ss")+
            "</h1>\n"
            "</div>"+
            ((flags&FlagInOneFrame)
                ?"<iframe src=\""+filePrefix+"a.html\" id=\"a-page\" "
                    "style=\" position: absolute;left: 0px;width: calc(100% - 4px );top: 70px;height: calc(100% - 75px );\">\n"
                    "</iframe>\n"
                :"<iframe src=\""+filePrefix+"a.html\" id=\"a-page\" "
                    "style=\" position: absolute;left: 0px;width: calc(50% - 2px );top: 70px;height: calc(100% - 75px );\">\n"
                    "</iframe>\n"
            "<iframe src=\""+filePrefix+"b.html\" id=\"b-page\" "
                    "style=\" position: absolute;left: 50%;width: calc(50% - 4px );top: 70px;height: calc(100% - 75px );\"></iframe>\n");
    QString document = makeHtml(body,filePrefix,pageWriteiterator);
    QFile fileMain (fileDirPrefix+"main.html");
    openFile.insert(fileDirPrefix+"main.html");
    fileMain.open( QIODevice::WriteOnly);
    fileMain.write((document).toUtf8());
    fileMain.close();
    QFile fileA (fileDirPrefix+"a.html");
    openFile.insert(fileDirPrefix+"a.html");
    fileA.open( QIODevice::WriteOnly);
    if(flags & FlagInOneFrame)
        fileA.write(generateCombinationIframe(flags).toUtf8());
    else
        fileA.write(generateIframe(0,flags).toUtf8());
    fileA.close();
    if(!(flags & FlagInOneFrame))
    {
        QFile fileB (fileDirPrefix+"b.html");
        openFile.insert(fileDirPrefix+"b.html");
        fileB.open(QIODevice::WriteOnly);
        fileB.write(generateIframe(1,flags).toUtf8());
        fileB.close();
    }
    QFile fileAct  (fileDirPrefix+"act.js");
    openFile.insert(fileDirPrefix+"act.js");
    fileAct.open(QIODevice::WriteOnly);
    fileAct.write(QString(QString("")+"ifnewact("+QString::number(pageWriteiterator)+")").toUtf8());
    fileAct.close();
}

QString PageComparator::generateIframe(int id, int flags)
{
    if(flags & FlagCourceCode)
    {
        QString out="<!DOCTYPE HTML><HTML><head></head><body>\n";
        QString beforeDiff = "<span style=\"background-color:red\">";
        QString afterDiff =  "</span>";
        for(int i=0;i<std::min(data.size(),dataFile.size());i++)
        {
            if(dataFile[i]&(1<<id))
            {
                QString toReplace=data_f[id][i];
                toReplace.replace("&","&#38;").replace("<","&#60").replace(">","&#62;").replace("\n","<br/>").replace(" ","&nbsp; ");
                 out+=(dataFile[i]==3?"":beforeDiff)
                         +toReplace
                         +(dataFile[i]==3?"":afterDiff)+"\n";
            }
        }
        return out+"\n</body>";
    }
    else
    {
        QString out;
        QString beforeDiff = "<span style=\"background-color:red\">";
        QString afterDiff =  "</span>";
        for(int i=0;i<std::min(data.size(),dataFile.size());i++)
        {
            if(data[i][0] == '<')
            {
                if(dataFile[i]&(1<<id)) out+=data_f[id][i];
            }
            else
            {
                if(dataFile[i]&(1<<id)) out+=(dataFile[i]==3?"":beforeDiff)+data_f[id][i]+(dataFile[i]==3?"":afterDiff)+"\n";
            }
        }
        return out;
    }
}

void PageComparator::writeTagBlock(QString & out,int file,int b,int e,QString before, QString after)
{
    for(int i=b;i<=e;i++)
        if(dataFile[i]&file)
        {
            if(data[i][0] == '<')
                out+=data[i];
            else
                out+=before+data[i]+after;
        }
}

#define WRITEA \
{if(dataFile[i] & (1<<0))\
{\
    if(closeTag[0][i]==-1)\
    writeTagBlock(out,1<<0,i,i,beforeDiff1,afterDiff);\
    else\
    {\
    writeTagBlock(out,1<<0,i,closeTag[0][i],beforeDiff1,afterDiff);\
    skip_a = closeTag[0][i];\
    }\
}}
#define WRITEB  \
{if(dataFile[i] & (1<<1))\
{\
    if(closeTag[1][i]==-1)\
    writeTagBlock(out,1<<1,i,i,beforeDiff2,afterDiff);\
    else\
    {\
    writeTagBlock(out,1<<1,i,closeTag[1][i],beforeDiff2,afterDiff);\
    skip_b = closeTag[1][i];\
    }\
}}
QString PageComparator::generateCombinationIframe(int flags)
{
    if(flags & FlagCourceCode)
    {
        QString out="<!DOCTYPE HTML><HTML><head></head><body>\n";
        QString beforeDiff1 = "<span style=\"background-color:red\">";
        QString beforeDiff2 = "<span style=\"background-color:green\">";
        QString afterDiff =  "</span>";
        for(int i=0;i<std::min(data.size(),dataFile.size());i++)
        {
            if(dataFile[i]&3)
            {
                QString toReplace=data[i];
                toReplace.replace("&","&#38;").replace("<","&#60").replace(">","&#62;").replace("\n","<br/>");
                 out+=(dataFile[i]==3?"":dataFile[i]==1?beforeDiff1:beforeDiff2)
                         +toReplace
                         +(dataFile[i]==3?"":afterDiff)+"\n";
            }
        }
        return out+"\n</body>";
    }
    else
    {
       D_PAGECOMPARATOR
       {
           QString toWrite;
           QFile debugFile ("comp/.debug.txt");
           debugFile.open(QIODevice::WriteOnly);
           for(int i=0;i<data.size();i++)
           {
               toWrite += QString::number(dataFile[i]) +"|"+QString::number(closeTag[0][i])+"|"+QString::number(closeTag[1][i])
                     +" \"" + QString(data[i]).replace("\n","\\n") + "\"\n";
           }
           debugFile.write(toWrite.toUtf8());
           debugFile.close();
       }
        QString out;
        QString beforeDiff1 = "<span style=\"background-color:red\">";
        QString beforeDiff2 = "<span style=\"background-color:green\">";
        QString afterDiff =  "</span>";
        int skip_a=-1, skip_b=-1;
        for(int i=0;i<std::min(data.size(),dataFile.size());i++)
        {
            if(dataFile[i]&3)
            {
                if(skip_a>=i)
                {
                    if(skip_b>=i)
                    {

                    }
                    else
                        WRITEB
                }
                else
                {
                    if(skip_b>=i)
                        WRITEA
                    else
                    {
                        if(((dataFile[i] & 3) == 3) && closeTag[0][i]==closeTag[1][i])
                            writeTagBlock(out,3,i,i,"","");
                        else
                        {
                            WRITEA
                            WRITEB
                        }
                    }
                }
            }
        }
        return out;
    }
}

int PageComparator::genCloseTag(int p)
{
    D_PAGECOMPARATOR qDebug() <<"\n\n\n genCloseTag:"<< p;
    QList<int> & out = closeTag[p];
    out.clear();
    QList<int> stuck;
    QList<QString> stuckTag;
    for(int i=0;i<data.size();i++) out.push_back(-1);
    for(int i=data.size()-1;i>=0;i--)
    {
        if(dataFile[i] & (1<<p))
        {
            QString s = data_f[p][i].trimmed();
            if(s[0]=='<' && !substrComp(s,0,"<script") && !substrComp(s,0,"<!--"))
            {
               QString tag = s.mid(1,s.length()-1-1).trimmed();
               D_PAGECOMPARATOR qDebug() <<"tag: "<< tag;
               if(tag[s.length()-1]=='/') continue;
               if(tag[0] == '/')
               {
                   stuck.push_back(i);
                   stuckTag.push_back(tag.mid(1).trimmed().split(" ")[0]);
               }
               else
               {
                   if(stuck.size() && tag.trimmed().split(" ")[0] == stuckTag.last())
                   {
                       D_PAGECOMPARATOR qDebug() <<"ISPAIR";
                       out[i]=stuck.last();
                       out[stuck.last()]=i;
                       stuck.pop_back();
                       stuckTag.pop_back();
                   }
                   else
                       D_PAGECOMPARATOR qDebug() <<tag.trimmed().split(" ")[0]<<"!="<<(stuck.size()?stuckTag.last():"NULPTR");
               }
            }
        }
    }
    return bool(stuck.size());
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
    data_f[0].clear();
    data_f[1].clear();
    dataFile.clear();
    QStringList a = loadFile(page_a);
    QStringList b = loadFile(page_b);
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
            if(i+1<a.size()&&j+1<b.size()) if(a[i+1].trimmed()==b[j+1].trimmed()) act = std::min(act,strDiffLen[i+1][j+1]);
            if(i+1==a.size()&&j+1==b.size()) act = 0;
            strDiffLen[i][j]=act;
        }
    }
    D_PAGECOMPARATOR qDebug()<<"PageComparator load calc arr ok";
    for(int i=0,j=0;;)
    {
        int act = strDiffLen[i][j];
        if(i+1<a.size()&&j+1<b.size()) if(a[i+1].trimmed()==b[j+1].trimmed()) if(act == strDiffLen[i+1][j+1])
        {
            i++;j++;
           data << a[i];
           data_f[0] << a[i];
           data_f[1] << b[j];
           dataFile << ((1<<1)|(1<<0));
           continue;
        }
        if(i+1<a.size()) if(act == strDiffLen[i+1][j]+1)
        {
            i++;
           data << a[i];
           data_f[0] << a[i];
           data_f[1] << "";
           dataFile << ((1<<0));
           continue;
        }
        if(j+1<b.size()) if(act == strDiffLen[i][j+1]+1)
        {
           j++;
           data << b[j];
           data_f[0] << "";
           data_f[1] << b[j];
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
   D_PAGECOMPARATOR
   {
       QString toWrite;
       QFile debugFile ("comp/.debug.txt");
       debugFile.open(QIODevice::WriteOnly);
       for(int i=0;i<data.size();i++)
       {
           toWrite += QString::number(dataFile[i]) + " \"" + QString(data[i]).replace("\n","\\n") + "\"\n";
       }
       debugFile.write(toWrite.toUtf8());
       debugFile.close();
   }
   genCloseTag(0);
   genCloseTag(1);
}

#define PUSH {if(!empty) {out << "";empty=1;}}
QStringList PageComparator::parseData(QString in)
{
    QStringList out = (QStringList()<< ""<<"");
    bool empty = 1;
    bool inTag=0;
    bool inScript=0;
    bool inComent=0;
//    bool lastSpace=0;
    for(int i=0;i<in.length();i++)
    {
        if(!inTag && !inComent && !inScript && (in[i].isSpace())) PUSH;
        if(!inTag && !inComent && !inScript && substrComp(in,i,"<script")) {PUSH;inScript=1;}
        if(!inTag && !inComent && !inScript && substrComp(in,i,"<!--")) {PUSH;inComent=1;}
        if(!inTag && !inComent && !inScript && in[i]=='<' ) {PUSH;inTag=1;}

        out[out.size()-1]+=in[i];
        if(!in[i].isSpace()) empty=0;

        if(in[i]=='>'&&inTag) {PUSH;inTag=0;}
        if(substrComp(in,i-8,"</script>")&&inScript) {PUSH;inScript=0;}
        if(substrComp(in,i-2,"-->")      &&inComent) {PUSH;inComent=0;}
    }
    for(int i=0;i<out.length();i++)
    {
        out[i]=out[i].trimmed();
        if(i+1<out.length())
        {
            int p=0;
            for(p=0;p<out[i+1].size() && out[i+1][p].isSpace();p++) ;
            out[i] += out[i+1].mid(0,p);
            out[i+1] = out[i+1].mid(p);
        }
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
        QStringList out = parseData(dat);
       D_PAGECOMPARATOR
       {
           QString toWrite;
           QFile debugFile ("comp/.debug"+in.fileName+".txt");
           debugFile.open(QIODevice::WriteOnly);
           for(int i=0;i<out.size();i++)
           {
           toWrite += "\"" + QString(out[i]).replace("\n","\\n") + "\"\n";
           }
           debugFile.write(toWrite.toUtf8());
           debugFile.close();
       }
       return out;
    }
    return QStringList()<<"";
}
