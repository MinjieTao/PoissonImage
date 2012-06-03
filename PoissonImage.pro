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
    data.cpp \
    sparsematrixsolver.cpp \
    jacobi.cpp

HEADERS  += mainwindow.h \
    sourceimage.h \
    destinationimage.h \
    maskimage.h \
    data.h \
    sparsematrixsolver.h \
    jacobi.h

FORMS    += mainwindow.ui

win32: LIBS += -L$$PWD/ -llibpardiso412-WIN-X86

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32: PRE_TARGETDEPS += $$PWD/libpardiso412-WIN-X86.lib
