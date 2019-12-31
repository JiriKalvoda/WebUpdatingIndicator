#include "mainwindow.h"
#include <debug.h>
#include <QScrollBar>
#include <QMenuBar>
#include <QMenu>
#include <historywindow.h>
MainWindow::MainWindow(int argc, char *argv[],QWidget *parent):MainWindow(parent)
{
    for(int i=0;i<argc;i++)
    {
   //console->setText(console->text()+"\n"+argv[i]);
   if(QString("stayHide")==QString(argv[i])) setWindowState(Qt::WindowMinimized);

    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);

    bg = new Background();
    if(bg->errorStarting()) {close(); return;}
    basicLayout = new QVBoxLayout();
    basicWidget = new QWidget();
    consoleScr  = new QScrollArea();
    console = new QLabel();
    bar     = new QProgressBar ();
    newPages = new PageViewer(bg);
    consoleBox = new QGroupBox();
    consoleLayout = new QHBoxLayout();

    buttonL = new QHBoxLayout();
    start = new QPushButton();
    start->setText("Start checking");
    connect(start,SIGNAL(pressed()),bg,SLOT(actualization()));
    buttonL->addWidget(start);
    stop  = new QPushButton();
    stop->setText("Stop checking");
    connect(stop,SIGNAL(pressed()),bg,SLOT(stopActualization()));
    buttonL->addWidget(stop);
    timeInfo = new QLabel();
    buttonL->addWidget(timeInfo);
    actPeriod = new QSpinBox ();
    connect(actPeriod,SIGNAL(valueChanged(int)),bg,SLOT(changeActPeriod(int)));
    actPeriod->setPrefix("Act. period: ");
    actPeriod->setSuffix(" min");
    actPeriod->setRange(0,60*24*7);
    buttonL->addWidget(actPeriod);


    connect(&(bg->bar),SIGNAL(change()),this,SLOT(setBar()));
    connect(&(bg->console),SIGNAL(change()),this,SLOT(setConsole()));
    connect(&(bg->barColor),SIGNAL(change()),this,SLOT(changeBarColor()));
    connect(&(bg->timeLastOk),SIGNAL(change()),this,SLOT(changeTimeInfo()));
    connect(&(bg->timeNextRun),SIGNAL(change()),this,SLOT(changeTimeInfo()));
    console->setStyleSheet("QLabel { background-color : black; color : white; }");
    console->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    newPages->setModel(&(bg->newPages));


    basicLayout->addWidget(newPages);
    basicLayout->addWidget(bar);
    basicLayout->addLayout(buttonL);
    consoleScr->setWidget(console);
    consoleScr->setWidgetResizable(1);
    connect(consoleScr->verticalScrollBar(),SIGNAL(rangeChanged(int,int)),
            this,SLOT(sliderRangeChanged(int,int)));
    //basicLayout->addWidget(consoleScr);
    basicLayout->addWidget(consoleBox);
    consoleBox->setCheckable(1);
    consoleBox->setChecked(0);
    consoleBox->setTitle("View more information");
    consoleBox->setLayout(consoleLayout);
    consoleLayout->setMargin(0);
    connect(consoleBox,SIGNAL(toggled(bool)),this,SLOT(viewConsole(bool)));

    basicWidget->setLayout(basicLayout);
    setCentralWidget(basicWidget);

    {
        auto pages = menuBar()->addMenu("pages");
        QAction * pages_history = pages->addAction("history");
        connect(pages_history,SIGNAL(triggered(bool)),this,SLOT(makeHistoryWindow(bool)));
        auto app = menuBar()->addMenu("app");
        QAction * app_quit = app->addAction("quit");
        connect(app_quit,SIGNAL(triggered(bool)),this,SLOT(exit()));
    }


    changeBarColor();
    changeTimeInfo();
    setBar();
    setConsole();
    changeActPeriod();

    resize(700,800);
    setWindowIcon(logo);
    connect(bg,SIGNAL(pageChanged_signal(NewPageItem)),this,SLOT(redIcon()));
    //setWindowFlags(Qt::CoverWindow);
//    stico = new QSystemTrayIcon(this);
//    stico->setIcon(QIcon("Logo.png"));
//    stico->show();
//    stico->showMessage("title","msq");
//    connect(stico,SIGNAL(messageClicked()),this,SLOT(jumpOnView()));
//    connect(stico,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(jumpOnView()));
}

MainWindow::~MainWindow()
{

}
void MainWindow::setBar()
{
    bar->setRange(0,100);
    bar->setValue(bg->bar->toInt());
}
void MainWindow::redIcon()
{
    activateWindow();
}

void MainWindow::setConsole()
{
    console->setText(bg->console->toString());
    /*consoleScr->verticalScrollBar()->setValue
            (consoleScr->verticalScrollBar()->maximum());
    consoleScr->verticalScrollBar()->setValue
            (consoleScr->verticalScrollBar()->maximum());*/
    //D_MAINWINDOW qDebug() << consoleScr->horizontalScrollBar()->minimum();
}
void MainWindow::sliderRangeChanged(int min, int max)
{
    Q_UNUSED(min);
    QScrollBar *bar = consoleScr->verticalScrollBar();
    bar->setValue(max);
}

void MainWindow::changeBarColor()
{
    bar->setStyleSheet(QString("::chunk {background-color: ")+bg->barColor->toString()+" }");
    bar->setTextVisible(0);
}
void MainWindow::changeTimeInfo()
{
    timeInfo->setText(QString("Last sucesfull act: ")+bg->timeLastOk->toDateTime().toString("d.M.yy h:mm")+"\n"
                              "Next actualization: "+bg->timeNextRun->toDateTime().toString("d.M.yy h:mm"));
    D_MAINWINDOW qDebug()<<"changeTimeInfo";
}
void MainWindow::changeActPeriod()
{
    actPeriod->setValue(bg->actPeriod->toInt());
}
void MainWindow::jumpOnView()
{
    //auto w = new waitForObject("{type='QMainWindow' visible='1'}");
    //
    //auto mb = new QMessageBox;
    //mb->show();
    //mb->clickedButton()->click();
    //clickButton(waitForObject("{text='OK' type='QPushButton' visible='1'}"));
    //snooze(0.1);
    //setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    //raise();  // for MacOS
    //activateWindow(); // for Windows
    //setFocus();
    //requestActivate();
    //stico->showMessage("title",QTime::currentTime().toString("mm:ss"));
    //stico->showMessage("title 2",QTime::currentTime().toString("mm:ss --2"));
    //setWindowState(Qt::WindowMaximized);
    //hide();
    //show();
    //D_MAINWINDOW qDebug()<<"------------------JUMPONVIEW";
}
void MainWindow::makeHistoryWindow(bool)
{
    D_MAINWINDOW qDebug()<<"makeHistoryWindow";
    (new HistoryWindow(bg))->show();
}

#include <QCloseEvent>
#include <QMessageBox>
void MainWindow::closeEvent (QCloseEvent *event)
{
        event->accept();
}
void MainWindow::exit()
{
    qApp->quit();
}
void MainWindow::viewConsole(bool isYes)
{
    while (consoleLayout->takeAt(0) != 0)
        consoleLayout->removeItem (consoleLayout->takeAt(0));
    if(isYes)
        consoleLayout->addWidget(consoleScr);
}
