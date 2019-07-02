#include "pagecomparatorgui.h"
#include <debug.h>
#include <QApplication>
#include <QDesktopWidget>

PageComparatorGui::PageComparatorGui(int a, int b,Background * bg, QWidget *parent):
    QWidget(parent),comp(a,b,bg)
{

    D_PAGECOMPARATOR qDebug() << "COMPARE " << a <<"; "<< b;
    l = new QVBoxLayout;
    button_generate = new QPushButton;
    button_generate->setText("Generate");
    connect(button_generate,SIGNAL(clicked(bool)),this,SLOT(generate()));
    l->addWidget(button_generate);
    button_open = new QPushButton;
    button_open->setText("Open");
    connect(button_open,SIGNAL(clicked(bool)),&comp,SLOT(open()));
    l->addWidget(button_open);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    check_sourceCode = new QCheckBox("View source code");
    l->addWidget(check_sourceCode);

    setLayout(l);

    generate();
    comp.open();

    setAttribute(Qt::WA_DeleteOnClose);
}
void PageComparatorGui::showInGoodPlace()
{
    auto disSize =QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(this));
    move(disSize.width()/2 - geometry().width()/2,0);
}

void PageComparatorGui::generate()
{
    comp.generate(check_sourceCode->isChecked());


}
