TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lgtest -pthread

SOURCES += main.cpp \
    SoundRecognizer_test.cpp \
    ../../SoundRecognizer.cpp

