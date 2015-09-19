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

signals:

public slots:

private:
    QAudioEncoderSettings settings_;

};

#endif // RECORDER_HPP
