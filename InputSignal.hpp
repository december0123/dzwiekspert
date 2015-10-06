#ifndef INPUT_SIGNAL_HPP
#define INPUT_SIGNAL_HPP

#include "FFT.hpp"
#include "FFTBuffer.hpp"
#include "recorder.hpp"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <QAudioProbe>
#include <QObject>

class InputSignal : public QObject
{
    Q_OBJECT
public:
    InputSignal();

    FFT fft_;
    Recorder recorder_;
    QAudioProbe probe_;
    std::condition_variable ready_;
    std::mutex m_;

    bool fftIsReady();

    int getFreq() const;
    void capture(bool capture);

private:
    std::atomic<bool> contCapture_{false};
    std::atomic<int> freq_{7};
    std::atomic<bool> fftReady{false};
    void processBuffer(QAudioBuffer buf);
};

#endif // INPUT_SIGNAL_HPP
