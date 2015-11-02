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
    QAudioProbe probe_;
    SoundRecognizer s_;
    std::condition_variable ready_;
    std::mutex m_;

    bool fftIsReady() const;

    std::vector<Note> getNotesAndInvalidate();
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

    std::vector<Note> findStrongestNotes(FFTBuffer& buf) const;
    Note getNote(const Note& idealNote);

private:
    std::vector<Note> strongestNotes_;
    std::atomic<bool> fftReady{false};
    constexpr static int LOWER_BOUND_FREQ{65};
    constexpr static int UPPER_BOUND_FREQ{1000};
    unsigned samplesBufferCounter_{0U};
    constexpr static unsigned NUM_OF_STRONGEST_NOTES_TO_FIND{5U};
    constexpr static unsigned FFT_THRESHOLD{100U};
    constexpr static float OVERLAP_FACTOR{0.5};
    FFTBuffer internalBuffer_;

private slots:
    void processBuffer(QAudioBuffer buf);
};

#endif // INPUT_SIGNAL_HPP
