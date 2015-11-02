#ifndef RECORDER_HPP
#define RECORDER_HPP

#include "Utils.hpp"

#include <QAudioEncoderSettings>
#include <QAudioRecorder>
#include <QAudioProbe>

class Recorder : public QAudioRecorder
{
    Q_OBJECT
public:
    Recorder();
    QAudioProbe probe_;
    const Frequency SAMPLING_FREQ{16000};
    const Frequency NYQUIST_FREQ{SAMPLING_FREQ * 0.5L};

private:
    QAudioEncoderSettings settings_;
};

#endif // RECORDER_HPP
