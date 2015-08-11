#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    QObject::connect(ui.go_to_chords, &QPushButton::clicked,
                     this, &MainWindow::go_to_chords);
    QObject::connect(ui.exit, &QPushButton::clicked,
                     this, &MainWindow::close);
}

void MainWindow::go_to_chords()
{
    ui.views->setCurrentIndex(CHORDS_VIEW);
}
