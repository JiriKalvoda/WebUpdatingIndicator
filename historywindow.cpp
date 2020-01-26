#include "historywindow.h"

HistoryWindow::HistoryWindow(Background * BG,QWidget * parent):QWidget(parent)
{
    bg = BG;
    label = new QLabel;
   l = new QVBoxLayout;
   label->setText("<span style=\" font-size:20pt; font-weight:600;\">Access to history of pages changes.</span>");
   l->addWidget(label);

   inputL = new QHBoxLayout;
   inputPrefix = new QLabel;
   inputPrefix->setText("select * from newPage <u>w</u>here ");
   inputL->addWidget(inputPrefix);
   inputInput = new QLineEdit;
   inputInput->setText("1");
   inputL->addWidget(inputInput);
   inputSufix = new QLabel;
   inputSufix->setText("");
   inputL->addWidget(inputPrefix);
   inputButton = new QPushButton;
   inputL->addWidget(inputButton);
   inputButton->setText("&Calculate");
   connect(inputButton,SIGNAL(clicked(bool)),this,SLOT(load()));
   connect(inputInput,SIGNAL(returnPressed()),this,SLOT(load()));
   l->addLayout(inputL);
   viewer = new PageViewer(bg);
   connect(viewer,SIGNAL(needActualization()),this,SLOT(reload()));
   l->addWidget(viewer);
   setLayout(l);

   setAttribute(Qt::WA_DeleteOnClose);

    qApp->installEventFilter(this);
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

bool HistoryWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        int k = key->key();
        if(key->key()==Qt::Key_Escape) setFocus();
        if(obj == this || obj == viewer || obj == viewer->table)
        {
        qDebug() << "KEY in HistoryWindow";
            if(k==Qt::Key_C)
                inputButton->click();
            if(k==Qt::Key_W)
                inputInput->setFocus();
            if(k==Qt::Key_Q)
                close();
            else
            {
                if(obj != viewer && obj != viewer->table)
                    viewer->eventFilter(viewer,event);
                return QObject::eventFilter(obj, event);
            }
            return true;

        }
    }
    return QObject::eventFilter(obj, event);
}
