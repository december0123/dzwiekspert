#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <QAudioEncoderSettings>
#include <QAudioRecorder>
#include <QAudioProbe>

class Recorder : public QAudioRecorder
{
    Q_OBJECT
public:
    Recorder();
    QAudioProbe probe_;
    const int SAMPLING_FREQ = 16000;
    const int NYQUIST_FREQ = SAMPLING_FREQ * 0.5;

private:
    QAudioEncoderSettings settings_;
};

#endif // RECORDER_HPP
