#include "mainwindow.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    QObject::connect(ui.goToTuner, &QPushButton::clicked,
                     this, &MainWindow::goToTuner);
    QObject::connect(ui.exit, &QPushButton::clicked,
                     this, &MainWindow::close);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setFreqIndicColor);
    QObject::connect(ui.startTuner, &QPushButton::toggled,
                     this, &MainWindow::startTuner);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setNoteInfo);
    QObject::connect(ui.startTuner, &QPushButton::toggled,
                     this, &MainWindow::setCaptureButtonText);
    QObject::connect(ui.goToMenu, &QPushButton::clicked,
                     this, &MainWindow::goToMenu);
    ui.views->setCurrentIndex(MENU_VIEW);
}

void MainWindow::goToTuner()
{
    ui.views->setCurrentIndex(TUNER_VIEW);
}

void MainWindow::goToMenu()
{
    turnOffTuner();
    ui.views->setCurrentIndex(MENU_VIEW);
}

void MainWindow::turnOffTuner()
{
    ui.freqIndicator->setValue(0);
    ui.freqIndicator->setPalette(this->style()->standardPalette());
    ui.note->setText(tr(""));
    CONTINUE_ = false;
    sig_.setCapture(false);
    ui.startTuner->setChecked(false);
}

void MainWindow::startTuner(bool cont)
{
    CONTINUE_ = cont;
    sig_.setCapture(cont);
    if (CONTINUE_)
    {
        std::thread t{&MainWindow::keepUpdatingFreqIndicator, this};
        t.detach();
    }
}

void MainWindow::keepUpdatingFreqIndicator()
{
    while(CONTINUE_)
    {
        auto f = sig_.getFreq();

        auto w = freqToVal(f);
        qDebug() << f << w;
        ui.freqIndicator->setValue(w);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    turnOffTuner();
}

void MainWindow::setNoteInfo(const int value)
{
    if (value >= UPPER_YELLOW || value <= BOTTOM_YELLOW)
    {
        ui.note->setText(tr("Żopa!"));
    }
    else if (value >= UPPER_GREEN || value <= BOTTOM_GREEN)
    {
        ui.note->setText(tr("Prawie!"));
    }
    else
    {
        ui.note->setText(tr("Super!"));
    }
}

void MainWindow::setCaptureButtonText(bool checked)
{
    if (checked)
    {
        ui.startTuner->setText(tr("Stop"));
    }
    else
    {
        ui.startTuner->setText(tr("Start"));
    }
}

int MainWindow::freqToVal(const int freq) const
{
    const auto ideal = 440;
    const auto value = 50 + ((ideal - freq) * 2);
    qDebug() << value;
    return std::min(value, UPPER_RED);
}

void MainWindow::setFreqIndicColor(const int freqVal)
{
    if (freqVal < UPPER_RED && freqVal > BOTTOM_RED)
    {
        auto pal = QPalette(Qt::green);
        if (freqVal < BOTTOM_YELLOW || freqVal > UPPER_YELLOW)
        {
            pal = QPalette{Qt::red};
        }
        else if (freqVal < BOTTOM_GREEN || freqVal > UPPER_GREEN)
        {
            pal = QPalette(Qt::yellow);
        }
        ui.freqIndicator->setPalette(pal);
    }
}
