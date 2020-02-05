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
    MainWindow(QWidget *parent = 0, int argc = 0, char ** argv = nullptr);
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
    QHBoxLayout * actPeriodLayout;
    QLabel * actPeriodLabel;
    QGroupBox * consoleBox;

    QIcon logo = QIcon("Logo.ico");
    //QSystemTrayIcon * stico;

 //   QStandardItemModel model;
    //NewPageModel newPageModel;

    //virtual void keyPressEvent(QKeyEvent *event);

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
    void about();
    void aboutQt();
    void openKeyHelp();

    void closeEvent (QCloseEvent *event);
    void exit();

protected:
     bool eventFilter(QObject *obj, QEvent *event);
     static QString  keyHelp();
};

#endif // MAINWINDOW_H
