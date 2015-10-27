#include "InputSignal.hpp"

#include <QDebug>

InputSignal::InputSignal()
{
    probe_.setSource(&recorder_);
    QObject::connect(&probe_, &QAudioProbe::audioBufferProbed,
            this, &InputSignal::processBuffer);
}

bool InputSignal::fftIsReady() const
{
    return fftReady.load();
}

Note InputSignal::getNoteAndInvalidate()
{
//    std::lock_guard<std::mutex> l{m_};
    fftReady.store(false);
    return note_;
}

void InputSignal::capture(bool capture)
{
    if (capture && recorder_.isAvailable())
    {
        recorder_.record();
    }
    else
    {
        recorder_.stop();
        analyser_.clear();
        qDebug() << "Recorder stop segfault";
    }
}

void InputSignal::processBuffer(QAudioBuffer buf)
{
    FFTBuffer fft_in{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
    analyser_.appendToBuff(fft_in);

    if (analyser_.FFTIsReady())
    {
        qDebug() << "fftisReady()";
        auto fft_buff = analyser_.getFFTBuffer();
        auto max = getMaxReal(fft_buff);
        long double biggest = std::distance(fft_buff.begin(), max);
        auto w = static_cast<long double>(recorder_.NYQUIST_FREQ) / static_cast<long double>(fft_buff.size()) * biggest;
        note_ = s_.recognizeNote(w);
        ready_.notify_all();
        fftReady.store(true);
    }
}
