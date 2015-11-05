#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include "Analyser.hpp"
#include "InputSignal.hpp"
#include "Note.hpp"
#include "recorder.hpp"
#include "Utils.hpp"

#include <QAudioBuffer>
#include <QAudioProbe>
#include <QDebug>

#include <atomic>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() = default;
    void gowno()
    {
        qDebug() << "gowno";
    }

private:
    Ui::MainWindow ui;

    const int MENU_VIEW = 0;
    const int TUNER_VIEW = 1;
    const int LEARN_VIEW = 2;
    int CURRENT_VIEW = MENU_VIEW;

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
    Note idealNote_{E2};

    std::atomic<bool> CONTINUE_{false};

    InputSignal sig_;

    int calcError(const int ideal, const int freq) const;
    void connectSlots();
    int noteToVal(const Note &note) const;
    void turnOffTuner();
    void setIdealNote();


private slots:
    void goToMenu();
    void goToTuner();
    void goToLearn();
    void keepUpdating();
    void setFreqIndicColor(const int freqVal);
    void setNoteInfo(const int value);
    void record(const bool cont);
    void setRandomNote();
signals:
    void noteChanged(QString note);
    void valueChanged(int val);
};

#endif // MAINWINDOW_HPP
