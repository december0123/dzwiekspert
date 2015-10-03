#include "recorder.hpp"

Recorder::Recorder()
{
    settings_.setChannelCount(1);
    settings_.setCodec("");
    settings_.setQuality(QMultimedia::NormalQuality);
    settings_.setEncodingMode(QMultimedia::ConstantQualityEncoding);
    settings_.setSampleRate(samplingFreq);
    setEncodingSettings(settings_);
}
