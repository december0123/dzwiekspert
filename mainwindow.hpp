#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() { qDebug() << "Żegnam\n"; }

public slots:
    void go_to_chords();

private:
    Ui::MainWindow ui;
    constexpr static auto MENU_VIEW = 0U;
    constexpr static auto CHORDS_VIEW = 1U;
};

#endif // MAINWINDOW_HPP
