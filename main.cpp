#include "mainwindow.hpp"
#include <QApplication>
#include <gtest/gtest.h>

#define TESTING

int main(int argc, char *argv[])
{
#ifndef TESTING
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
#else
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
}
