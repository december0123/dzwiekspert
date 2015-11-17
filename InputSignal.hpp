#ifndef INPUT_SIGNAL_HPP
#define INPUT_SIGNAL_HPP

#include "Analyser.hpp"
#include "FFTBuffer.hpp"
#include "Note.hpp"
#include "recorder.hpp"
#include "SoundRecognizer.hpp"
#include "Utils.hpp"

#include <QAudioProbe>
#include <QObject>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

class InputSignal : public QObject
{
    Q_OBJECT
public:
    InputSignal();

    SoundRecognizer recognizer_;

    bool fftIsReady() const;
    std::vector<Note> getNotesAndInvalidate();
    void capture(bool capture);
    std::vector<Note> findStrongestNotes(FFTBuffer& buf) const;
    Note getNote(const Note& idealNote);
    void setBasicA(const Frequency f);

private:
    std::vector<Note> strongestNotes_;
    std::atomic<bool> fftReady{false};
    constexpr static int LOWER_BOUND_FREQ{65};
    constexpr static int UPPER_BOUND_FREQ{1000};
    unsigned samplesBufferCounter_{0U};
    constexpr static unsigned NUM_OF_STRONGEST_NOTES_TO_FIND{5U};
    constexpr static unsigned FFT_THRESHOLD{150U};
    constexpr static float OVERLAP_FACTOR{0.5};
    FFTBuffer internalBuffer_;
    std::mutex m_;
    Recorder recorder_;
    QAudioProbe probe_;
    std::condition_variable ready_;

private slots:
    void processBuffer(QAudioBuffer buf);
};

#endif // INPUT_SIGNAL_HPP
