#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <QAudioEncoderSettings>
#include <QAudioRecorder>

class Recorder : public QAudioRecorder
{
public:
    Recorder();

signals:

public slots:

private:
    QAudioEncoderSettings settings_;
};

#endif // RECORDER_HPP
