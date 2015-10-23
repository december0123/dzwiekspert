#include "mainwindow.hpp"
#include <QApplication>
#include <gtest/gtest.h>
#include "InputSignal.hpp"

//#define TESTING_ON


int main(int argc, char *argv[])
{
#ifndef TESTING_ON
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
#else
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
}
