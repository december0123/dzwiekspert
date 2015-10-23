#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T20:01:07
#
#-------------------------------------------------


QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Inzynierka
TEMPLATE = app

CONFIG += c++14
QMAKE_CXXFLAGS += "-Wextra"
QMAKE_CXXFLAGS_RELEASE += "-Werror"

LIBS += -lgtest -pthread

SOURCES += main.cpp\
        mainwindow.cpp \
    InputSignal.cpp \
    recorder.cpp \
    kissfft/kiss_fft.c \
    FFTBuffer.cpp \
    Analyser.cpp \
    SoundRecognizer.cpp \
    SoundRecognizer_test.cpp \
    Note_test.cpp

HEADERS  += mainwindow.hpp \
    InputSignal.hpp \
    recorder.hpp \
    kissfft/kiss_fft.h \
    kissfft/_kiss_fft_guts.h \
    FFTBuffer.hpp \
    Analyser.hpp \
    Utils.hpp \
    SoundRecognizer.hpp \
    Note.hpp

FORMS    += mainwindow.ui
