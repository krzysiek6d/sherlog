#-------------------------------------------------
#
# Project created by QtCreator 2021-09-13T16:50:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aaa
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    logarea.cpp \
    mytab.cpp \
    tabcontainer.cpp \
    config.cpp \
    documenttab.cpp

HEADERS += \
    mainwindow.h \
    logarea.h \
    filecontents.h \
    tabcontainer.h \
    config.h \
    mytab.h \
    timer.h \
    documenttab.h

FORMS += \
    mainwindow.ui \
    mytab.ui \
    tabcontainer.ui \
    documenttab.ui

CONFIG += static
#CONFIG += sanitizer sanitize_address

QMAKE_CXXFLAGS += -std=c++2a
QMAKE_LFLAGS += -static-libstdc++
