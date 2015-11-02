#include "mainwindow.hpp"

#include <gtest/gtest.h>

#include <QApplication>

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
