#include "pagecomparatorgui.h"
#include <debug.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QKeyEvent>

PageComparatorGui::PageComparatorGui(int a, int b,Background * bg, QWidget *parent):
    QWidget(parent),comp(a,b,bg)
{

    D_PAGECOMPARATOR qDebug() << "COMPARE " << a <<"; "<< b;
    l = new QVBoxLayout;
    button_generate = new QPushButton;
    button_generate->setText("&Generate");
    connect(button_generate,SIGNAL(clicked(bool)),this,SLOT(generate()));
    l->addWidget(button_generate);
    button_open = new QPushButton;
    button_open->setText("&Open");
    connect(button_open,SIGNAL(clicked(bool)),&comp,SLOT(open()));
    l->addWidget(button_open);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle("WebUpdatingIndicator compare");

    check_sourceCode = new QCheckBox("View source &code");
    l->addWidget(check_sourceCode);
    check_oneFrame = new QCheckBox("&Two page in one");
    l->addWidget(check_oneFrame);

    setLayout(l);

    generate();
    comp.open();

    setAttribute(Qt::WA_DeleteOnClose);

    qApp->installEventFilter(this);
}
void PageComparatorGui::showInGoodPlace()
{
    auto disSize =QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(this));
    move(disSize.width()/2 - geometry().width()/2,0);
}

void PageComparatorGui::generate()
{
    comp.generate(
            check_sourceCode->isChecked() * comp.FlagCourceCode |
            check_oneFrame->isChecked() * comp.FlagInOneFrame
        );


}

bool PageComparatorGui::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        int k = key->key();
        if(k==Qt::Key_Escape)
        {
            setFocus();
        }
        if(obj == this)
        {
            if(k==Qt::Key_Q)
                close();
            else if(k==Qt::Key_G)
                button_generate->animateClick();
            else if(k==Qt::Key_O)
                button_open->animateClick();
            else if(k==Qt::Key_C)
                check_sourceCode->animateClick();
            else if(k==Qt::Key_T)
                check_oneFrame->animateClick();
            else
            {
                return QObject::eventFilter(obj, event);
            }
            return 1;
        }
    }
    return QObject::eventFilter(obj, event);
}
