#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include <QDebug>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QMediaPlayer>
#include <mutex>

#include "InputSignal.hpp"
#include "recorder.hpp"


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
    const int MIDDLE_VAL = 50;

    std::mutex m_;
    std::atomic<bool> CONTINUE_{false};
    InputSignal sig_;
    QAudioBuffer buf_;

    int freqToVal(const int freq) const;
    void turnOffTuner();
    void connectSlots();
    int calcScaledError(const int ideal, const int freq) const;
    void setUpRecorder();
    Recorder audioRecorder;
    QMediaPlayer player;

private slots:
    void goToTuner();
    void goToMenu();
    void setFreqIndicColor(const int freqVal);
    void setTunerState(bool cont);
    void keepUpdatingFreqIndicator();
    void setNoteInfo(const int value);
    void setCaptureButtonText(bool checked);
    void startRecord();
    void stopRecord();
    void goToRecord();
    void processBuffer(QAudioBuffer buf);
    void play();
};

#endif // MAINWINDOW_HPP
