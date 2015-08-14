#include "InputSignal.hpp"
#include <QDebug>
#include <random>
#include <thread>

InputSignal::InputSignal()
{

}

unsigned InputSignal::getFreq() const
{
    return freq_;
}

void InputSignal::toggleCapture()
{
    contCapture_ = !contCapture_;
    if (contCapture_)
    {
        std::thread t{&InputSignal::capture, this};
        t.detach();
    }
}

void InputSignal::stop()
{
    contCapture_ = false;
}
void InputSignal::capture()
{
    std::uniform_int_distribution<> distr{400, 460};
    std::mt19937_64 eng{std::random_device{}()};
    while(contCapture_)
    {
        freq_ = distr(eng);
    }
}
