#include "mainwindow.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <thread>

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

    QObject::connect(this, &MainWindow::valueChanged,
                     ui.freqIndicator, &QSlider::setValue);
    QObject::connect(this, &MainWindow::noteChanged,
                     ui.currFreq, &QLabel::setText);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setFreqIndicColor);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setNoteInfo);

    QObject::connect(ui.tunerStateBtn, &QPushButton::toggled,
                     this, &MainWindow::setTunerState);

    QObject::connect(ui.startRecord, &QPushButton::clicked,
                     this, &MainWindow::startRecord);
    QObject::connect(ui.stopRecord, &QPushButton::clicked,
                     this, &MainWindow::stopRecord);
    QObject::connect(ui.goToRecord, &QPushButton::clicked,
                     this, &MainWindow::goToRecord);

    QObject::connect(ui.tune_e2, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_a3, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_d4, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_g4, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_b4, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_e5, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
}

int MainWindow::calcError(const int ideal, const int freq) const
{
    return (freq - ideal);
}

int MainWindow::noteToVal(const Note& note) const
{
    return MIDDLE_VAL + calcError(idealNote_.getFreq(), note.getFreq());
}

void MainWindow::turnOffTuner()
{
    CONTINUE_.store(false);
    ui.freqIndicator->setValue(MIDDLE_VAL);
    ui.freqIndicator->setPalette(this->style()->standardPalette());
    ui.note->setText(tr("Włącz stroik"));
    ui.tunerStateBtn->setChecked(false);
}

void MainWindow::setIdealNote()
{
    if (ui.tune_e2->isChecked())
    {
        idealNote_ = E2;
    }
    else if (ui.tune_a3->isChecked())
    {
        idealNote_ = A2;
    }
    else if (ui.tune_d4->isChecked())
    {
        idealNote_ = D3;
    }
    else if (ui.tune_g4->isChecked())
    {
        idealNote_ = G3;
    }
    else if (ui.tune_b4->isChecked())
    {
        idealNote_ = B3;
    }
    else if (ui.tune_e5->isChecked())
    {
        idealNote_ = E4;
    }
}

void MainWindow::goToMenu()
{
    turnOffTuner();
    ui.views->setCurrentIndex(MENU_VIEW);
}

void MainWindow::goToTuner()
{
    ui.views->setCurrentIndex(TUNER_VIEW);
}

void MainWindow::goToRecord()
{
    ui.views->setCurrentIndex(2);
}

void MainWindow::keepUpdatingFreqIndicator()
{
    sig_.capture(true);
    while(CONTINUE_.load())
    {
        std::unique_lock<std::mutex> l(sig_.m_);
        sig_.ready_.wait(l, [&](){return sig_.fftIsReady();});
        emit valueChanged(noteToVal(sig_.getNoteAndInvalidate()));
        emit noteChanged(QString::fromStdString(sig_.note_.getName()));
    }
    qDebug() << "Continue false";
    sig_.capture(false);
    turnOffTuner();
}

void MainWindow::setFreqIndicColor(const int freqVal)
{
    auto pal = QPalette(Qt::green);
    if ((freqVal < UPPER_RED) && (freqVal > LOWER_RED))
    {
        if ((freqVal < LOWER_YELLOW) || (freqVal > UPPER_YELLOW))
        {
            pal = QPalette{Qt::red};
        }
        else if ((freqVal < LOWER_GREEN) || (freqVal > UPPER_GREEN))
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

void MainWindow::setNoteInfo(const int value)
{
    if (value >= UPPER_YELLOW || value <= LOWER_YELLOW)
    {
        ui.note->setText(tr("Żopa!"));
    }
    else if (value >= UPPER_GREEN || value <= LOWER_GREEN)
    {
        ui.note->setText(tr("Prawie!"));
    }
    else
    {
        ui.note->setText(tr("Super!"));
    }
}

void MainWindow::setTunerState(const bool cont)
{
    qDebug() << cont;
    CONTINUE_.store(cont);
    if (CONTINUE_.load())
    {
        std::thread t{&MainWindow::keepUpdatingFreqIndicator, this};
        t.detach();
        ui.tunerStateBtn->setText(tr("Stop"));
    }
    else
    {
        qDebug() << "Powinno wylaczyc CONTINUE";
        ui.tunerStateBtn->setText(tr("Start"));
    }
}

void MainWindow::startRecord()
{
    sig_.capture(true);
}

void MainWindow::stopRecord()
{
    sig_.capture(false);
}
