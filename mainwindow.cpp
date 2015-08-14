#include "mainwindow.hpp"

#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    QObject::connect(ui.go_to_tuner, &QPushButton::clicked,
                     this, &MainWindow::go_to_tuner);
    QObject::connect(ui.exit, &QPushButton::clicked,
                     this, &MainWindow::close);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setFreqIndicColor);
    QObject::connect(ui.inc, &QPushButton::clicked,
                     this, &MainWindow::incrementFreqIndicator);
    QObject::connect(ui.dec, &QPushButton::clicked,
                     this, &MainWindow::decrementFreqIndicator);
    QObject::connect(ui.test, &QPushButton::toggled,
                     this, &MainWindow::swiruj);
    QObject::connect(ui.stopTest, &QPushButton::clicked,
                     this, &MainWindow::stopTest);
    ui.views->setCurrentIndex(MENU_VIEW);
    ui.freqIndicator->setPalette(Qt::green);
}

void MainWindow::go_to_tuner()
{
    ui.views->setCurrentIndex(TUNER_VIEW);
}

void MainWindow::swiruj(bool cont)
{
    CONTINUE_ = cont;
    sig_.toggleCapture();
    if (CONTINUE_)
    {
        std::thread t{&MainWindow::test, this};
        t.detach();
    }

}

void MainWindow::test()
{
    while(CONTINUE_)
    {
        ui.freqIndicator->setValue(freq2val(sig_.getFreq()));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void MainWindow::stopTest()
{
    CONTINUE_ = false;
    sig_.toggleCapture();
}

unsigned MainWindow::freq2val(unsigned freq) const
{
    auto ideal = 440;
    auto value = 50 + ((ideal - freq) * 2);
    if (value > 99)
    {
        return 99;
    }
    return value;
}

void MainWindow::incrementFreqIndicator()
{
    const auto freq_val = ui.freqIndicator->value();
    ui.freqIndicator->setValue(freq_val + 1);
}

void MainWindow::decrementFreqIndicator()
{
    const auto freq_val = ui.freqIndicator->value();
    ui.freqIndicator->setValue(freq_val - 1);
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
