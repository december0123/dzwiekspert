#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <QAudioEncoderSettings>
#include <QAudioRecorder>
#include <QAudioProbe>

class Recorder : public QAudioRecorder
{
public:
    Recorder();
    QAudioProbe probe_;
    const int samplingFreq = 16000;
    const int nyquistFreq = samplingFreq * 0.5;

private:
    QAudioEncoderSettings settings_;
};

#endif // RECORDER_HPP
