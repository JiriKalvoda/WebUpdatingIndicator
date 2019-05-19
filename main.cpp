#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Background bg;
    MainWindow w(argc, argv);
    w.show();
    //bg.actualization();
    return a.exec();
}
