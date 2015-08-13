#include "mainwindow.hpp"

#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    QObject::connect(ui.go_to_tuner, &QPushButton::clicked,
                     this, &MainWindow::go_to_chords);
    QObject::connect(ui.exit, &QPushButton::clicked,
                     this, &MainWindow::close);
    QObject::connect(ui.freq_indicator, &QSlider::valueChanged,
                     this, &MainWindow::set_freq_indic_color);
    QObject::connect(ui.pushButton, &QPushButton::clicked,
                     this, &MainWindow::increment_freq_indicator);
    QObject::connect(ui.pushButton_2, &QPushButton::clicked,
                     this, &MainWindow::decrement_freq_indicator);
    QObject::connect(ui.pushButton_3, &QPushButton::clicked,
                     this, &MainWindow::swiruj);
    ui.views->setCurrentIndex(MENU_VIEW);
    ui.freq_indicator->setPalette(Qt::green);
}

void MainWindow::go_to_chords()
{
    ui.views->setCurrentIndex(TUNER_VIEW);
}

void MainWindow::swiruj()
{
    for (int i = 0; i < 10; ++i)
    {
        for(int i = 0; i < 100; ++i)
        {
            increment_freq_indicator();
            ui.freq_indicator->repaint();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        for(int i = 0; i < 100; ++i)
        {
            decrement_freq_indicator();
            ui.freq_indicator->repaint();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void MainWindow::increment_freq_indicator()
{
    const auto freq_val = ui.freq_indicator->value();
    ui.freq_indicator->setValue(freq_val + 1);

    qDebug() << ui.freq_indicator->value();
}

void MainWindow::decrement_freq_indicator()
{
    const auto freq_val = ui.freq_indicator->value();
    ui.freq_indicator->setValue(freq_val - 1);

    qDebug() << ui.freq_indicator->value();
}

void MainWindow::set_freq_indic_color(int freq_val)
{
    if (freq_val < 99 && freq_val > 0)
    {
        auto pal = QPalette(Qt::green);
        if (freq_val < 35 || freq_val > 65)
        {
            pal = QPalette{Qt::red};
        }
        else if (freq_val < 45 || freq_val > 55)
        {
            pal = QPalette(Qt::yellow);
        }
        ui.freq_indicator->setPalette(pal);
    }
}
