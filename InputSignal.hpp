#ifndef INPUT_SIGNAL_HPP
#define INPUT_SIGNAL_HPP

#include "Analyser.hpp"
#include "FFTBuffer.hpp"
#include "recorder.hpp"
#include "SoundRecognizer.hpp"

#include <QAudioProbe>
#include <QObject>

#include <atomic>
#include <condition_variable>
#include <mutex>

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

    Note getNoteAndInvalidate();
    void capture(bool capture);
    auto getMaxReal(FFTBuffer& buf) const
    {
        return std::max_element(std::next(buf.begin(), LOWER_BOUND_FREQ), std::next(buf.begin(), UPPER_BOUND_FREQ),
                                [&](const auto& lhs, const auto& rhs){return lhs.r < rhs.r;});
    }
    Note note_;
private:

    std::atomic<bool> fftReady{false};
    // 8000 czestotliwosci
    // 3200 rozmiar bufora
    // 2.5 na jeden, wiec 26 == 65Hz
    const int LOWER_BOUND_FREQ{26};
    const int UPPER_BOUND_FREQ{800};
    unsigned samplesBufferCounter_ = 0;
    const unsigned FFT_THRESHOLD = 40;
    const unsigned OVERLAP_FACTOR = FFT_THRESHOLD / 2;
    FFTBuffer outputBuff_;
    FFTBuffer internalBuffer_;

    void processBuffer(QAudioBuffer buf);
};

#endif // INPUT_SIGNAL_HPP
