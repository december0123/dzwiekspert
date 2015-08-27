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
    constexpr static auto MENU_VIEW = 0U;
    constexpr static auto TUNER_VIEW = 1U;
    constexpr static auto RED = 255U;
    constexpr static auto GREEN = 0U;
    constexpr static auto UPPER_GREEN = 55U;
    constexpr static auto BOTTOM_GREEN = 45U;
    constexpr static auto UPPER_YELLOW = 65U;
    constexpr static auto BOTTOM_YELLOW = 35U;
    constexpr static auto UPPER_RED = 99U;
    constexpr static auto BOTTOM_RED = 0U;

    bool CONTINUE_ = false;
    InputSignal sig_;

    unsigned freqToVal(unsigned freq) const;
    void turnOffTuner();

private slots:
    void goToTuner();
    void goToMenu();
    void setFreqIndicColor(unsigned freqVal);
    void startTuner(bool cont);
    void updateFreqIndicator();
    void setNoteInfo(unsigned value);
    void setCaptureButtonText(bool checked);
};

#endif // MAINWINDOW_HPP
