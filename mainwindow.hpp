#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include <QDebug>
#include <QAudioBuffer>
#include <QAudioProbe>

#include "InputSignal.hpp"
#include "Note.hpp"
#include "recorder.hpp"
#include "Analyser.hpp"
#include "Utils.hpp"

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
    const int UPPER_GREEN = 55;
    const int LOWER_GREEN = 45;
    const int UPPER_YELLOW = 65;
    const int LOWER_YELLOW = 35;
    const int UPPER_RED = 99;
    const int LOWER_RED = 0;
    const int MIDDLE_VAL = 50;
    const Note E2{"E2", 82.41_Hz};
    const Note A2{"A2", 110.00_Hz};
    const Note D3{"D3", 146.83_Hz};
    const Note G3 {"G3", 196.00_Hz};
    const Note B3{"B3", 246.94_Hz};
    const Note E4{"E4", 329.63_Hz};
    Note idealFreq_ = E2;

    std::atomic<bool> CONTINUE_{false};
    InputSignal sig_;

    int calcError(const int ideal, const int freq) const;
    void connectSlots();
    int freqToVal(const int freq) const;
    void turnOffTuner();
    void setIdealFreq();

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
