#ifndef INPUT_SIGNAL_HPP
#define INPUT_SIGNAL_HPP

#include <atomic>

class InputSignal
{
public:
    InputSignal() = default;
    ~InputSignal() = default;
    unsigned getFreq() const;
    void setCapture(bool active);
private:
    int freq_{440};
    bool contCapture_{false};
    void capture();
};

#endif // INPUT_SIGNAL_HPP
