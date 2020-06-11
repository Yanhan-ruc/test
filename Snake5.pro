#-------------------------------------------------
#
# Project created by QtCreator 2020-04-23T16:46:36
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Snake.cpp

HEADERS  += mainwindow.h \
    Snake.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    media.qrc
