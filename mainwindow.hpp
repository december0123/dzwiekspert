#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include "Analyser.hpp"
#include "ConfigParser.hpp"
#include "InputSignal.hpp"
#include "Note.hpp"
#include "SoundRecognizer.hpp"
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

private:
    Ui::MainWindow ui;

    enum VIEWS {MENU = 0, TUNER = 2, LEARN = 3, CONFIG = 1};

    int CURRENT_VIEW{VIEWS::MENU};

    const int UPPER_GREEN{55};
    const int LOWER_GREEN{45};
    const int UPPER_YELLOW{65};
    const int LOWER_YELLOW{35};
    const int UPPER_RED{99};
    const int LOWER_RED{0};
    const int MIDDLE_VAL{50};

    ConfigParser configs_{"config"};
    Note idealNote_;

    std::atomic<bool> CONTINUE_{false};

    InputSignal sig_;
    SoundRecognizer recognizer_;

    int calcError(const int ideal, const int freq) const;
    void connectSlots();
    int noteToVal(const Note &note) const;
    void turnOffTuner();
    void setIdealNote();

private slots:
    void goToMenu();
    void goToTuner();
    void goToLearn();
    void goToConfig();
    void keepUpdating();
    void setFreqIndicColor(const int freqVal);
    void setNoteInfo(const int value);
    void record(const bool cont);
    void setRandomNote();
    void saveConfig();

signals:
    void noteChanged(QString note);
    void valueChanged(int val);
};

#endif // MAINWINDOW_HPP
