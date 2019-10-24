#ifndef MAINWINDOW_H
#define MAINWINDOW_H

 #include <QMainWindow>
 #include <QVBoxLayout>
 #include <QHBoxLayout>
 #include <QWidget>
#include <QLabel>
 #include <QStandardItemModel>
#include <QProgressBar>
#include <QScrollArea>
#include <QGroupBox>

 //#include "deletetableviewer.h"
 #include "background.h"
#include <QTableView>
#include <QPushButton>
#include <QSpinBox>

#include <QSystemTrayIcon>
#include <qmessagebox.h>
#include <QCloseEvent>
#include "pageviewer.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    MainWindow(int argc, char *argv[],QWidget *parent = 0);
    ~MainWindow();

    //QTableView viewer;
//    DeleteTableViewer viewer;
    QVBoxLayout * basicLayout;
    QWidget * basicWidget;

    PageViewer * newPages;
    QScrollArea * consoleScr;
    QHBoxLayout * consoleLayout;
    QLabel * console;
    QProgressBar *  bar;

    QHBoxLayout * buttonL;
    QPushButton * start;
    QPushButton * stop;
    QLabel * timeInfo;
    QSpinBox * actPeriod;
    QGroupBox * consoleBox;

    QIcon logo = QIcon("Logo.ico");
    //QSystemTrayIcon * stico;

 //   QStandardItemModel model;
    //NewPageModel newPageModel;



    Background * bg;
public slots:
    void setBar();
    void setConsole();
    void sliderRangeChanged(int,int);
    void changeBarColor();
    void changeTimeInfo();
    void changeActPeriod();
    void jumpOnView();
    void redIcon();
    void makeHistoryWindow(bool);
    void viewConsole(bool isYes);

    void closeEvent (QCloseEvent *event);
    void exit();
};

#endif // MAINWINDOW_H
