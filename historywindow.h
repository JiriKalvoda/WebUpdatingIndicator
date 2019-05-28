#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QWidget>
#include <mainwindow.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <qlineedit.h>
#include <pageviewer.h>
#include <pagequery.h>
#include <background.h>
class HistoryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryWindow(Background * BG, QWidget *parent = 0);
    QVBoxLayout * l;
    QLabel      * label;
    QHBoxLayout * inputL;
    QLabel      * inputPrefix;
    QLineEdit   * inputInput;
    QLabel      * inputSufix;
    QPushButton * inputButton;
    QString lastQueryString;
    QString error;
    PageViewer * viewer;
    PageQuery * query = 0;
    Background * bg;

signals:

public slots:
    void load();
    void reload();
};

#endif // HISTORYWINDOW_H
