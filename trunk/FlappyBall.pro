#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T21:17:13
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlappyBall
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mygraphicsview.cpp

HEADERS  += mainwindow.h \
    mygraphicsview.h

FORMS    +=

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    res.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES +=

