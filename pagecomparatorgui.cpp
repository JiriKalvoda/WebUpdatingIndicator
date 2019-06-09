#include "pagecomparatorgui.h"
#include <debug.h>

PageComparatorGui::PageComparatorGui(int a, int b,Background * bg, QWidget *parent):
    QWidget(parent),comp(a,b,bg)
{

    D_PAGECOMPARATOR qDebug() << "COMPARE " << a <<"; "<< b;
    l = new QHBoxLayout;
    button_generate = new QPushButton;
    button_generate->setText("Generate");
    connect(button_generate,SIGNAL(clicked(bool)),&comp,SLOT(generate(bool)));
    l->addWidget(button_generate);
    button_open = new QPushButton;
    button_open->setText("Open");
    connect(button_open,SIGNAL(clicked(bool)),&comp,SLOT(open()));
    l->addWidget(button_open);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    setLayout(l);

    comp.generate(0);
    comp.open();

    setAttribute(Qt::WA_DeleteOnClose);

}
