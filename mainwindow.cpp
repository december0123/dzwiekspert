#include "mainwindow.hpp"

#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    connectSlots();
    ui.views->setCurrentIndex(MENU_VIEW);
    ui.note->setText(tr("Włącz stroik"));
}

void MainWindow::connectSlots()
{
    QObject::connect(ui.exit, &QPushButton::clicked,
                     this, &MainWindow::close);
    QObject::connect(ui.goToMenu, &QPushButton::clicked,
                     this, &MainWindow::goToMenu);
    QObject::connect(ui.goToTuner, &QPushButton::clicked,
                     this, &MainWindow::goToTuner);

    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setFreqIndicColor);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setNoteInfo);

    QObject::connect(ui.tunerStateBtn, &QPushButton::toggled,
                     this, &MainWindow::setTunerState);
    QObject::connect(ui.tunerStateBtn, &QPushButton::toggled,
                     this, &MainWindow::setCaptureButtonText);

    QObject::connect(ui.startRecord, &QPushButton::clicked,
                     this, &MainWindow::startRecord);
    QObject::connect(ui.stopRecord, &QPushButton::clicked,
                     this, &MainWindow::stopRecord);
    QObject::connect(ui.stopRecord, &QPushButton::clicked,
                     this, &MainWindow::play);
    QObject::connect(ui.goToRecord, &QPushButton::clicked,
                     this, &MainWindow::goToRecord);

}

void MainWindow::goToRecord()
{
    ui.views->setCurrentIndex(2);
}

void MainWindow::play()
{
    player.setMedia(QUrl::fromLocalFile(ui.url->text()));
    player.setVolume(50);
    player.play();
}

void MainWindow::startRecord()
{
    sig_.capture(true);
}

void MainWindow::stopRecord()
{
    sig_.capture(false);
}

int MainWindow::calcScaledError(const int ideal, const int freq) const
{
    return (ideal - freq);
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
    ui.freqIndicator->setValue(MIDDLE_VAL);
    ui.freqIndicator->setPalette(this->style()->standardPalette());
    ui.note->setText(tr("Włącz stroik"));
    CONTINUE_.store(false);
    ui.tunerStateBtn->setChecked(false);

}

void MainWindow::keepUpdatingFreqIndicator()
{
    sig_.capture(true);
    while(CONTINUE_.load())
    {
        std::unique_lock<std::mutex> l(sig_.m_);
        sig_.ready.wait(l, [&]
        {
            return sig_.fftIsReady();
        });
        auto f = sig_.getFreq();
        auto w = freqToVal(f);
        ui.freqIndicator->setValue(w);
    }
    sig_.capture(false);
    turnOffTuner();
}

void MainWindow::setTunerState(bool cont)
{
    CONTINUE_.store(cont);
    if (CONTINUE_.load())
    {
        std::thread t{&MainWindow::keepUpdatingFreqIndicator, this};
        t.detach();
    }
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
        ui.tunerStateBtn->setText(tr("Stop"));
    }
    else
    {
        ui.tunerStateBtn->setText(tr("Start"));
    }
}

int MainWindow::freqToVal(const int freq) const
{
    const auto ideal = 750;
    const auto value = MIDDLE_VAL + calcScaledError(ideal, freq);
    return std::min(value, UPPER_RED);
}

void MainWindow::setFreqIndicColor(const int freqVal)
{
    auto pal = QPalette(Qt::green);
    if ((freqVal < UPPER_RED) && (freqVal > BOTTOM_RED))
    {
        if ((freqVal < BOTTOM_YELLOW) || (freqVal > UPPER_YELLOW))
        {
            pal = QPalette{Qt::red};
        }
        else if ((freqVal < BOTTOM_GREEN) || (freqVal > UPPER_GREEN))
        {
            pal = QPalette(Qt::yellow);
        }
    }
    else
    {
        pal = QPalette(Qt::red);
    }
    ui.freqIndicator->setPalette(pal);
}
