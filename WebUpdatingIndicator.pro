#------------------------------------------------- q
#
# Project created by QtCreator 2019-04-20T13:19:07
#
#-------------------------------------------------

QT       += core gui sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebUpdatingIndicator
TEMPLATE = app


SOURCES += main.cpp\
       mainwindow.cpp \
     pageviewer.cpp \
 #   deleteproxy.cpp \
    background.cpp \
    conectionthread.cpp \
    monitoredvar.cpp \
    newpagemodel.cpp \
    historywindow.cpp \
    pagequery.cpp

HEADERS  += background.h \
    conectionthread.h \
mainwindow.h \
    monitoredvar.h \
    newpagemodel.h \
    pageviewer.h \
 #   deleteproxy.h \
    historywindow.h \
    pagequery.h

