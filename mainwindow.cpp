#include "mainwindow.hpp"

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
        std::thread t{&MainWindow::updateFreqIndicator, this};
        t.detach();
    }
}

void MainWindow::updateFreqIndicator()
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

void MainWindow::setNoteInfo(int value)
{
    if (value >= 65 || value <= 35)
    {
        ui.note->setText(tr("Żopa!"));
    }
    else if (value >= 55 || value <= 45)
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

unsigned MainWindow::freqToVal(unsigned freq) const
{
    auto ideal = 440;
    auto value = 50 + ((ideal - freq) * 2);
    qDebug() << value;
    if (value > 99)
    {
        return 99;
    }
    return value;
}

void MainWindow::setFreqIndicColor(int freqVal)
{
    if (freqVal < 99 && freqVal > 0)
    {
        auto pal = QPalette(Qt::green);
        if (freqVal < 35 || freqVal > 65)
        {
            pal = QPalette{Qt::red};
        }
        else if (freqVal < 45 || freqVal > 55)
        {
            pal = QPalette(Qt::yellow);
        }
        ui.freqIndicator->setPalette(pal);
    }
}
