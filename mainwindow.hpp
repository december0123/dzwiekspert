#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include <QDebug>
#include <QAudioBuffer>
#include <QAudioProbe>

#include "InputSignal.hpp"
#include "recorder.hpp"
#include "FFT.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() = default;

private:
    Ui::MainWindow ui;
    const int MENU_VIEW = 0;
    const int TUNER_VIEW = 1;
    const int RED = 255;
    const int GREEN = 0;
    const int UPPER_GREEN = 55;
    const int LOWER_GREEN = 45;
    const int UPPER_YELLOW = 65;
    const int LOWER_YELLOW = 35;
    const int UPPER_RED = 99;
    const int LOWER_RED = 0;
    const int MIDDLE_VAL = 50;

    std::atomic<bool> CONTINUE_{false};
    InputSignal sig_;

    int calcError(const int ideal, const int freq) const;
    void connectSlots();
    int freqToVal(const int freq) const;
    void turnOffTuner();

private slots:
    void goToMenu();
    void goToTuner();
    void goToRecord();
    void keepUpdatingFreqIndicator();
    void setCaptureButtonText(const bool checked);
    void setFreqIndicColor(const int freqVal);
    void setNoteInfo(const int value);
    void setTunerState(const bool cont);
    void startRecord();
    void stopRecord();

};

#endif // MAINWINDOW_HPP
