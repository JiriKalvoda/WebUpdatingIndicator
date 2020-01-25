#include "mainwindow.h"
#include <debug.h>
#include <QScrollBar>
#include <QMenuBar>
#include <QMenu>
#include <historywindow.h>

MainWindow::MainWindow(QWidget *parent, int argc, char ** argv)
    : QMainWindow(parent)
{
    for(int i=1;i<argc;i++)
    {
       //console->setText(console->text()+"\n"+argv[i]);
       if(QString("--stayHide")==QString(argv[i])) setWindowState(Qt::WindowMinimized);
    }
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint);
    bg = new Background(this,argc,argv);
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
    start->setText("&Start checking");
    connect(start,SIGNAL(pressed()),bg,SLOT(actualization()));
    buttonL->addWidget(start,1);
    stop  = new QPushButton();
    stop->setText("Stop ch&ecking");
    connect(stop,SIGNAL(pressed()),bg,SLOT(stopActualization()));
    buttonL->addWidget(stop,1);
    timeInfo = new QLabel();
    buttonL->addWidget(timeInfo,1);
    actPeriod = new QSpinBox ();
    actPeriodLabel = new QLabel ();
    actPeriodLayout = new QHBoxLayout ();
    connect(actPeriod,SIGNAL(valueChanged(int)),bg,SLOT(changeActPeriod(int)));
    actPeriodLabel->setText("Act. <u>p</u>eriod: ");
    actPeriod->setSuffix(" min");
    actPeriod->setRange(0,60*24*7);
    actPeriodLayout->addWidget(actPeriodLabel,0);
    actPeriodLayout->addWidget(actPeriod,1);
    buttonL->addLayout(actPeriodLayout,1);


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
        pages_history->setShortcut(Qt::Key_H);
        auto app = menuBar()->addMenu("app");
        QAction * app_quit = app->addAction("quit");
        app_quit->setShortcut(Qt::CTRL+Qt::Key_Q);
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

    qApp->installEventFilter(this);
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

/*
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    setFocusPolicy(Qt::WheelFocus);
    qDebug() << "MainWindow "<<event->text();
}
*/
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        if(key->key()==Qt::Key_Escape) setFocus();
        if(obj == this || obj == newPages || obj == newPages->table)
        {
            if(key->key()==Qt::Key_S) start->click();
            if(key->key()==Qt::Key_E) stop->click();
            if(key->key()==Qt::Key_P) actPeriod->setFocus();
        }
    }
    return QObject::eventFilter(obj, event);
}
