#ifndef INPUT_SIGNAL_HPP
#define INPUT_SIGNAL_HPP

#include <atomic>

class InputSignal
{
public:
    InputSignal();
    unsigned getFreq() const;
    void toggleCapture();
    void stop();
private:
    std::atomic<unsigned> freq_{0};
    bool contCapture_{false};
    void capture();
};

#endif // INPUT_SIGNAL_HPP
