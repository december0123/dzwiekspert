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
#include <deque>
#include <mutex>

class InputSignal : public QObject
{
    Q_OBJECT
public:
    InputSignal();

    Analyser analyser_;
    Recorder recorder_;
    Recorder noiseRecorder_;
    QAudioProbe probe_;
    QAudioProbe noiseProbe_;
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

    auto getSecondMaxReal(FFTBuffer& buf) const
    {
        auto max = getMaxReal(buf);

        auto nm = std::find_if(std::next(buf.begin(), LOWER_BOUND_FREQ), std::next(buf.begin(), UPPER_BOUND_FREQ),
                                [&](const auto& lhs){return lhs.r >= max->r * 0.9;});
        qDebug () << "Max: " << double(max->r) << "Nowe max: " << double(nm->r);
        if (nm == buf.end())
        {
            throw;
        }
        return nm;
    }
    Note note_;
private:

    std::atomic<bool> fftReady{false};
    std::deque<Note> lastNotes_;
    // 8000 czestotliwosci
    // 3200 rozmiar bufora
    // 2.5 na jeden, wiec 26 == 65Hz
    constexpr static int LOWER_BOUND_FREQ{11};
    constexpr static int UPPER_BOUND_FREQ{100};
    unsigned samplesBufferCounter_ = 0;
    constexpr static unsigned FFT_THRESHOLD = 20;
    constexpr static float OVERLAP_FACTOR = 0.5;
    FFTBuffer internalBuffer_;
    FFTBuffer noise_;

    void processBuffer(QAudioBuffer buf);
    void recordNoise();
    void processNoise(QAudioBuffer buf);
};

#endif // INPUT_SIGNAL_HPP
