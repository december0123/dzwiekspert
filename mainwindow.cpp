#include "mainwindow.hpp"

#include <algorithm>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    connectSlots();
    ui.views->setCurrentIndex(VIEWS::MENU);
    ui.note->setText(tr("Włącz stroik"));
    ui.goToMenu->hide();
    ui.toggleRecorder->hide();
    readConfig();
}

void MainWindow::connectSlots()
{
    QObject::connect(ui.exit, &QPushButton::clicked,
                     this, &MainWindow::close);
    QObject::connect(ui.goToMenu, &QPushButton::clicked,
                     this, &MainWindow::goToMenu);
    QObject::connect(ui.goToTuner, &QPushButton::clicked,
                     this, &MainWindow::goToTuner);
    QObject::connect(ui.goToLearn, &QPushButton::clicked,
                     this, &MainWindow::goToLearn);
    QObject::connect(ui.goToConfig, &QPushButton::clicked,
                     this, &MainWindow::goToConfig);

    QObject::connect(this, &MainWindow::valueChanged,
                     ui.freqIndicator, &QSlider::setValue);
    QObject::connect(this, &MainWindow::noteChanged,
                     ui.currFreq, &QLabel::setText);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setFreqIndicColor);
    QObject::connect(ui.freqIndicator, &QSlider::valueChanged,
                     this, &MainWindow::setNoteInfo);

    QObject::connect(ui.toggleRecorder, &QPushButton::toggled,
                     this, &MainWindow::record);

    QObject::connect(ui.tune_e2, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_a2, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_d3, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_g3, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_b3, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);
    QObject::connect(ui.tune_e4, &QRadioButton::clicked,
                     this, &MainWindow::setIdealNote);

    QObject::connect(ui.drawRandomNote, &QPushButton::clicked,
                     this, &MainWindow::setRandomNote);

    QObject::connect(ui.drawRandomNote, &QPushButton::toggled,
                     this, &MainWindow::record);

    for (auto& tuning : ui.configTuningGroup->findChildren<QRadioButton*>())
    {
        QObject::connect(tuning, &QRadioButton::clicked,
                         this, &MainWindow::saveConfig);
    }
    for (auto& basicARadio : ui.basicAGroup->findChildren<QRadioButton*>())
    {
        QObject::connect(basicARadio, &QRadioButton::clicked,
                         this, &MainWindow::saveConfig);
    }
    QObject::connect(ui.basic_customEdit, &QLineEdit::textChanged,
                     this, &MainWindow::saveConfig);

    QObject::connect(ui.basic_custom, &QRadioButton::toggled,
                     ui.basic_customEdit, &QLineEdit::setEnabled);
}

int MainWindow::calcError(const int ideal, const int freq) const
{
    qDebug() << ideal << "vs" << freq;
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
    ui.toggleRecorder->setChecked(false);
}

void MainWindow::setIdealNote()
{
    for (const auto& note : ui.tunerNotesGroup->findChildren<QRadioButton*>())
    {
        if (note->isChecked())
        {
            idealNote_ = recognizer_.findNote(note->text().toStdString());
            break;
        }
    }
}

void MainWindow::goToMenu()
{
    turnOffTuner();
    CURRENT_VIEW = VIEWS::MENU;
    ui.views->setCurrentIndex(CURRENT_VIEW);
    ui.goToMenu->hide();
    ui.toggleRecorder->hide();
}

void MainWindow::goToTuner()
{
    readConfig();
    CURRENT_VIEW = VIEWS::TUNER;
    ui.views->setCurrentIndex(CURRENT_VIEW);
    ui.goToMenu->show();
    ui.toggleRecorder->show();
    setIdealNote();
}

void MainWindow::goToLearn()
{
    readConfig();
    CURRENT_VIEW = VIEWS::LEARN;
    ui.views->setCurrentIndex(CURRENT_VIEW);
    ui.goToMenu->show();
    ui.toggleRecorder->show();
    setRandomNote();
}

void MainWindow::goToConfig()
{
    readConfig();
    CURRENT_VIEW = VIEWS::CONFIG;
    ui.views->setCurrentIndex(CURRENT_VIEW);
    ui.goToMenu->show();
}

void MainWindow::keepUpdating()
{
    sig_.capture(true);
    while(CONTINUE_.load())
    {
        Note currentNote = sig_.getNote(idealNote_);
        qDebug() << "WYkrylem: " << currentNote.getFullName().c_str();
        if (CURRENT_VIEW == VIEWS::TUNER)
        {
            emit valueChanged(noteToVal(currentNote));
            emit noteChanged(
                        QString::fromStdString(
                            std::to_string(currentNote.getFreq()) + " " + currentNote.getName()));
        }
        else if (CURRENT_VIEW == VIEWS::LEARN)
        {
            if (currentNote.getName() == idealNote_.getName())
            {
                ui.learnStatus->setText("Cacy");
            }
            else
            {
                ui.learnStatus->setText("Nie cacy");
            }
        }
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
        ui.note->setText(tr("Daleko!"));
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

void MainWindow::record(const bool cont)
{
    qDebug() << cont;
    CONTINUE_.store(cont);
    if (CONTINUE_.load())
    {
        std::thread t{&MainWindow::keepUpdating, this};
        t.detach();
        ui.toggleRecorder->setText(tr("Stop"));
    }
    else
    {
        qDebug() << "Powinno wylaczyc CONTINUE";
        ui.toggleRecorder->setText(tr("Start"));
    }
}

void MainWindow::setRandomNote()
{
    ui.noteToPlay->setText(QString::fromStdString(sig_.recognizer_.getRandomNote().getName()));
    idealNote_ = sig_.recognizer_.findNote(ui.noteToPlay->text().toStdString());
}

void MainWindow::readConfig()
{
    std::string basic{configs_.lookup("basic")};
    sig_.setBasicA(std::stold(basic));
    recognizer_.setBasicA(std::stold(basic));
    if (basic == ui.basic_440->text().toStdString())
    {
        ui.basic_440->setChecked(true);
    }
    else if (basic == ui.basic_432->text().toStdString())
    {
        ui.basic_432->setChecked(true);
    }
    else
    {
        ui.basic_custom->setChecked(true);
        ui.basic_customEdit->setText(basic.c_str());
    }

    std::string tuning{configs_.lookup("tuning")};
    for (const auto& tuningRadio : ui.configTuningGroup->findChildren<QRadioButton*>())
    {
        std::string t{tuningRadio->text().toStdString()};
        t = t.substr(t.size()-17);
        if (t == tuning)
        {
            tuningRadio->setChecked(true);
            break;
        }
    }

    std::deque<std::string> tunSounds = configs_.split(tuning, ",");
    ui.fretboard->setStrings(tunSounds);
    ui.tune_e2->setText(tunSounds[0].c_str());
    ui.tune_a2->setText(tunSounds[1].c_str());
    ui.tune_d3->setText(tunSounds[2].c_str());
    ui.tune_g3->setText(tunSounds[3].c_str());
    ui.tune_b3->setText(tunSounds[4].c_str());
    ui.tune_e4->setText(tunSounds[5].c_str());
    std::string hand{configs_.lookup("hand")};
    idealNote_ = recognizer_.findNote(tunSounds[0]);
}

void MainWindow::saveConfig()
{
    std::string option;
    if (ui.basic_440->isChecked())
    {
        option = ui.basic_440->text().toStdString();
    }
    else if (ui.basic_432->isChecked())
    {
        option = ui.basic_432->text().toStdString();
    }
    else if (ui.basic_custom->isChecked())
    {
        option = ui.basic_customEdit->text().toStdString();
    }
    configs_.write("basic", option);
    sig_.setBasicA(std::stold(option));

    for (const auto& tuningRadio : ui.configTuningGroup->findChildren<QRadioButton*>())
    {
        if (tuningRadio->isChecked())
        {
            option = tuningRadio->text().toStdString();
            break;
        }
    }
    configs_.write("tuning", option.substr(option.find("-") + 2));
    configs_.save();
}
