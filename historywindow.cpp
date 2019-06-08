#include "historywindow.h"

HistoryWindow::HistoryWindow(Background * BG,QWidget * parent):QWidget(parent)
{
    bg = BG;
    label = new QLabel;
   l = new QVBoxLayout;
   label->setText("history of pages");
   l->addWidget(label);

   inputL = new QHBoxLayout;
   inputPrefix = new QLabel;
   inputPrefix->setText("select * from newPage where ");
   inputL->addWidget(inputPrefix);
   inputInput = new QLineEdit;
   inputInput->setText("1");
   inputL->addWidget(inputInput);
   inputSufix = new QLabel;
   inputSufix->setText("");
   inputL->addWidget(inputPrefix);
   inputButton = new QPushButton;
   inputL->addWidget(inputButton);
   inputButton->setText("Load");
   connect(inputButton,SIGNAL(clicked(bool)),this,SLOT(load()));
   l->addLayout(inputL);
   viewer = new PageViewer(bg);
   connect(viewer,SIGNAL(needActualization()),this,SLOT(reload()));
   l->addWidget(viewer);
   setLayout(l);

   setAttribute(Qt::WA_DeleteOnClose);
}

void HistoryWindow::load()
{
    viewer->setModel(0);
    if(query) delete query;
    lastQueryString = inputInput->text();
    if(lastQueryString!="") query = bg->history(QString(" where ")+lastQueryString+"");
    viewer->setModel(query);
}
void HistoryWindow::reload()
{
    viewer->setModel(0);
    if(query) delete query;
    if(lastQueryString!="") query = bg->history(QString(" where ")+lastQueryString+"");
    viewer->setModel(query);
}
