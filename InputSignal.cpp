#include "InputSignal.hpp"
#include <QDebug>
#include <random>
#include <thread>

unsigned InputSignal::getFreq() const
{
    return freq_;
}

void InputSignal::setCapture(bool active)
{
    contCapture_ = active;
    if (contCapture_)
    {
        std::thread t{&InputSignal::capture, this};
        t.detach();
    }
}

void InputSignal::capture()
{
    std::uniform_int_distribution<int> distr{-1, 1};
    std::mt19937_64 eng{std::random_device{}()};
    while(contCapture_)
    {
        qDebug() << "START CAPTURE";
        auto w = distr(eng);
        qDebug() << w;
        freq_ += w;
        qDebug() << freq_;
        qDebug() << "END CAPTURE";
        if (freq_ < 400 || freq_ > 460)
        {
            freq_ = 440;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
