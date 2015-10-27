#include "recorder.hpp"

#include <QUrl>

Recorder::Recorder()
{
    settings_.setChannelCount(1);
    settings_.setCodec("");
    settings_.setQuality(QMultimedia::NormalQuality);
    settings_.setEncodingMode(QMultimedia::ConstantQualityEncoding);
    settings_.setSampleRate(SAMPLING_FREQ);
    setOutputLocation(QUrl::fromLocalFile("/dev/null"));
    setEncodingSettings(settings_);
}
