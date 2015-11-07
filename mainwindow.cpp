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
    ui.switchRecorder->hide();
    sig_.setBasic(std::stold(parser_.lookup("basic")));
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

    QObject::connect(ui.switchRecorder, &QPushButton::toggled,
                     this, &MainWindow::record);

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

    QObject::connect(ui.drawRandomNote, &QPushButton::clicked,
                     this, &MainWindow::setRandomNote);

    QObject::connect(ui.drawRandomNote, &QPushButton::toggled,
                     this, &MainWindow::record);

    QObject::connect(ui.saveConfig, &QPushButton::clicked,
                     this, &MainWindow::saveConfig);
    QObject::connect(ui.basic_custom, &QRadioButton::toggled,
                     ui.basic_customEdit, &QLineEdit::setEnabled);
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
    ui.switchRecorder->setChecked(false);
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
    CURRENT_VIEW = VIEWS::MENU;
    ui.views->setCurrentIndex(CURRENT_VIEW);
    ui.goToMenu->hide();
    ui.switchRecorder->hide();
}

void MainWindow::goToTuner()
{
    CURRENT_VIEW = VIEWS::TUNER;
    ui.views->setCurrentIndex(CURRENT_VIEW);
    ui.goToMenu->show();
    ui.switchRecorder->show();
    idealNote_ = E2;
}

void MainWindow::goToLearn()
{
    CURRENT_VIEW = VIEWS::LEARN;
    ui.views->setCurrentIndex(CURRENT_VIEW);
    ui.goToMenu->show();
    ui.switchRecorder->show();
    setRandomNote();
}

void MainWindow::goToConfig()
{
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
        ui.switchRecorder->setText(tr("Stop"));
    }
    else
    {
        qDebug() << "Powinno wylaczyc CONTINUE";
        ui.switchRecorder->setText(tr("Start"));
    }
}

void MainWindow::setRandomNote()
{
    ui.noteToPlay->setText(QString::fromStdString(sig_.recognizer_.getRandomNote().getFullName()));
    idealNote_ = sig_.recognizer_.findNote(ui.noteToPlay->text().toStdString());
}

void MainWindow::saveConfig()
{
    std::string option;
    if (ui.basic_65->isChecked())
    {
        option = ui.basic_65->text().toStdString();
    }
    else if (ui.basic_custom->isChecked())
    {
        option = ui.basic_customEdit->text().toStdString();
    }
    parser_.write("basic", option);
    sig_.setBasic(std::stold(option));

    if (ui.tuning_CLASSIC->isChecked())
    {
        option = ui.tuning_CLASSIC->text().toStdString();
    }
    else if (ui.tuning_DROP_D->isChecked())
    {
        option = ui.tuning_DROP_D->text().toStdString();
    }
    else if (ui.tuning_CELTIC->isChecked())
    {
        option = ui.tuning_CELTIC->text().toStdString();
    }
    else if (ui.tuning_OPEN_G->isChecked())
    {
        option = ui.tuning_OPEN_G->text().toStdString();
    }
    auto pos = option.find("-");
    parser_.write("tuning", option.substr(pos + 2));
    parser_.save();

}
