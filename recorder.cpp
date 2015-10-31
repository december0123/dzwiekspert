#include "recorder.hpp"

#include <QUrl>
#include <QAudioFormat>

Recorder::Recorder()
{
    settings_.setChannelCount(1);
    settings_.setCodec("");
    settings_.setQuality(QMultimedia::HighQuality);
    settings_.setEncodingMode(QMultimedia::ConstantQualityEncoding);
    settings_.setSampleRate(SAMPLING_FREQ);
    setOutputLocation(QUrl::fromLocalFile("/dev/null"));
    setEncodingSettings(settings_);
}
