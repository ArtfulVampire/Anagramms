#-------------------------------------------------
#
# Project created by QtCreator 2011-01-17T13:52:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11 #for C++11 standard

TARGET = AnagrammAnim

TEMPLATE = app


SOURCES += main.cpp

OTHER_FILES += \
    6letNew.txt

DISTFILES += \
    6letOld.txt \
    7let.txt
