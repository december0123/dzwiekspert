#ifndef INPUT_SIGNAL_HPP
#define INPUT_SIGNAL_HPP

#include "Analyser.hpp"
#include "FFTBuffer.hpp"
#include "recorder.hpp"
#include "SoundRecognizer.hpp"

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

    Analyser analyser_;
    Recorder recorder_;
    QAudioProbe probe_;
    SoundRecognizer s_;
    std::condition_variable ready_;
    std::mutex m_;

    bool fftIsReady() const;

    int getFreqAndInvalidate();
    void capture(bool capture);
    auto getMaxReal(FFTBuffer& buf) const
    {
        return std::max_element(std::next(buf.begin(), LOWER_BOUND_FREQ), std::next(buf.begin(), UPPER_BOUND_FREQ),
                                [&](const auto& lhs, const auto& rhs){return lhs.r < rhs.r;});
    }
    std::string note_;
private:
    std::atomic<bool> contCapture_{false};
    std::atomic<double> freq_{7};

    std::atomic<bool> fftReady{false};
    void processBuffer(QAudioBuffer buf);
    // 8000 czestotliwosci
    // 3200 rozmiar bufora
    // 2.5 na jeden, wiec 28 == 70Hz
    const int LOWER_BOUND_FREQ{28};
    const int UPPER_BOUND_FREQ{200};
};

#endif // INPUT_SIGNAL_HPP
