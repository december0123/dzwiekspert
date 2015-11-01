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
                                [&](const auto& lhs){return lhs.r >= max->r * 0.8;});
        return nm;
    }

    void findStrongestNotes(FFTBuffer& buf)
    {
        // basically, an array of length 5, initialized to the minimum integer
        std::vector<Note> maxima(3, Note{});
        // go through all numbers.
        for (unsigned long freqIndex = LOWER_BOUND_FREQ; freqIndex < UPPER_BOUND_FREQ; ++freqIndex)
        {
            // find smallest in maxima.
            auto smallestIndex = 0U;
            for (auto m = 0U; m != maxima.size(); ++m)
            {
                if (maxima[m].getFreq() < maxima[smallestIndex].getFreq())
                {
                    smallestIndex = m;
                }
            }

            // check if smallest is smaller than current number
            if (maxima[smallestIndex].getFreq() < buf[freqIndex].r && buf[freqIndex].r > 10000)
            {
                maxima[smallestIndex] = Note{"", 0, buf[freqIndex].r, static_cast<double>(freqIndex)};
            }
        }
        for (Note& n : maxima)
        {
            n = s_.recognizeNote(recorder_.NYQUIST_FREQ / buf.size() * n.getError());
        }
        notes_ = std::move(maxima);
        for (const Note& i : notes_)
        {
            qDebug() << QString::fromStdString(i.getName());
        }
    }
    std::vector<Note> notes_;
    Note note_;
private:

    std::atomic<bool> fftReady{false};

    constexpr static int LOWER_BOUND_FREQ{65};
    constexpr static int UPPER_BOUND_FREQ{1000};
    unsigned samplesBufferCounter_ = 0;
    constexpr static unsigned FFT_THRESHOLD = 100;
    constexpr static float OVERLAP_FACTOR = 0.5;
    FFTBuffer internalBuffer_;
    FFTBuffer noise_;

    void processBuffer(QAudioBuffer buf);
    void recordNoise();
    void processNoise(QAudioBuffer buf);
};

#endif // INPUT_SIGNAL_HPP
