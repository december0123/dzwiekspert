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

    volatile std::atomic<int> freq_{440};
    std::condition_variable ready;
    FFT fft_;
    QAudioProbe probe_;
    Recorder audioRecorder;
    std::mutex m_;

    bool fftIsReady()
    {
        if (fftReady)
        {
            fftReady = false;
            return !fftReady;
        }
        return fftReady;
    }

    int getFreq() const;
    void capture(bool capture);
private:

    std::atomic<bool> contCapture_{false};
    volatile bool fftReady{false};
    void processBuffer(QAudioBuffer buf);
};

#endif // INPUT_SIGNAL_HPP
