#ifndef INPUT_SIGNAL_HPP
#define INPUT_SIGNAL_HPP

#include <atomic>

class InputSignal
{
public:
    InputSignal();
    ~InputSignal() = default;
    int getFreq() const;
    void startCapture(bool active);
    volatile std::atomic<int> freq_{440};
private:

    std::atomic<bool> contCapture_{false};
    void capture();
};

#endif // INPUT_SIGNAL_HPP
