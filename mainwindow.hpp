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
    void swiruj();

private:
    Ui::MainWindow ui;
    constexpr static auto MENU_VIEW = 0U;
    constexpr static auto TUNER_VIEW = 1U;
    constexpr static auto RED = 255U;
    constexpr static auto GREEN = 0U;

private slots:
    void increment_freq_indicator();
    void decrement_freq_indicator();
    void set_freq_indic_color(int freq_val);
};

#endif // MAINWINDOW_HPP
