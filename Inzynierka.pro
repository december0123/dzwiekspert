#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T20:01:07
#
#-------------------------------------------------


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Inzynierka
TEMPLATE = app

CONFIG += c++14

LIBS += -lgtest -pthread

SOURCES += main.cpp\
        mainwindow.cpp \
    InputSignal.cpp

HEADERS  += mainwindow.hpp \
    InputSignal.hpp

FORMS    += mainwindow.ui
