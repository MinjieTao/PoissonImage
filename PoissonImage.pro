#-------------------------------------------------
#
# Project created by QtCreator 2012-05-30T23:32:33
#
#-------------------------------------------------

QT       += core gui debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PoissonImage
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sourceimage.cpp \
    destinationimage.cpp \
    maskimage.cpp \
    data.cpp

HEADERS  += mainwindow.h \
    sourceimage.h \
    destinationimage.h \
    maskimage.h \
    data.h

FORMS    += mainwindow.ui
