#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    QString style="Fusion";
    QApplication a(argc, argv);
    /*{
        QString s;
        for(int i=0;i<argc;i++)
            s+="|"+QString(argv[i])+"|\n";
        QMessageBox::information(0,"ARG",s);
    }*/
    for(int i=1;i<argc;i++)
    {
       //console->setText(console->text()+"\n"+argv[i]);
       if(QString("--setStyle")==QString(argv[i]))
       {
           style=argv[i+1];
           //QMessageBox::information(0,"Set style",style);
           //std::cout << "style: "<<argv[i+1] <<" "<< style.toStdString() <<std::endl;
       }
       if(QString("--styleHelp")==QString(argv[i]))
       {
           //std::cout<<("List of valid styles:")<<std::endl;
           QString s;
           for(QString it : QStyleFactory::keys())
           {
               s+= it +"\n";
               //std::cout<<it.toStdString()<<std::endl;
           }
           //std::cout<<std::flush;
           QMessageBox::information(0,"\nList of valid styles",s+"USE: --setStyle <STYLE>");
           return 0;
       }
    }
    a.setStyle(style);
    MainWindow w(0,argc, argv);
    w.show();
    return a.exec();
}
