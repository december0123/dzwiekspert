#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include "InputSignal.hpp"
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() { qDebug() << "Żegnam\n"; }

private:
    Ui::MainWindow ui;
    const int MENU_VIEW = 0;
    const int TUNER_VIEW = 1;
    const int RED = 255;
    const int GREEN = 0;
    const int UPPER_GREEN = 55;
    const int BOTTOM_GREEN = 45;
    const int UPPER_YELLOW = 65;
    const int BOTTOM_YELLOW = 35;
    const int UPPER_RED = 99;
    const int BOTTOM_RED = 0;

    bool CONTINUE_ = false;
    InputSignal sig_;

    int freqToVal(const int freq) const;
    void turnOffTuner();
    void connectSlots();

private slots:
    void goToTuner();
    void goToMenu();
    void setFreqIndicColor(const int freqVal);
    void startTuner(bool cont);
    void keepUpdatingFreqIndicator();
    void setNoteInfo(const int value);
    void setCaptureButtonText(bool checked);
};

#endif // MAINWINDOW_HPP
