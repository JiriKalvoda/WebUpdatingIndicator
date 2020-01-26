#------------------------------------------------- q
#
# Project created by QtCreator 2019-04-20T13:19:07
#
#-------------------------------------------------

QT       += core gui sql
QT += network
#QT += webengine
#QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebUpdatingIndicator
TEMPLATE = app


SOURCES += main.cpp\
    connectionthread.cpp \
       mainwindow.cpp \
     pageviewer.cpp \
 #   deleteproxy.cpp \
    background.cpp \
    monitoredvar.cpp \
    newpagemodel.cpp \
    historywindow.cpp \
    pagequery.cpp \
    pagecomparatorgui.cpp \
    pagecomparator.cpp \
    database.cpp

HEADERS  += background.h \
    connectionthread.h \
mainwindow.h \
    monitoredvar.h \
    newpagemodel.h \
    pageviewer.h \
 #   deleteproxy.h \
    historywindow.h \
    pagequery.h \
    pagecomparatorgui.h \
    pagecomparator.h \
    debug.h \
    database.h


VERSION = 2.2

DEFINES += APP_NAME=\"\\\"WebUpdatingindicator\\\"\"
DEFINES += APP_VERSION=\"\\\"$${VERSION}.\\\"\"
