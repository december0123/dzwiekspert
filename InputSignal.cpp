#include "InputSignal.hpp"
#include <QDebug>

InputSignal::InputSignal()
{
    probe_.setSource(&recorder_);
    QObject::connect(&probe_, &QAudioProbe::audioBufferProbed,
            this, &InputSignal::processBuffer);
}

bool InputSignal::fftIsReady()
{
    if (fftReady.load())
    {
        fftReady.store(false);
        return !fftReady.load();
    }
    return fftReady.load();
}

int InputSignal::getFreq() const
{
    qDebug() << "Oddalem " << freq_.load();
    return freq_.load();
}

void InputSignal::capture(bool capture)
{
    contCapture_.store(capture);
    if (capture && recorder_.isAvailable())
    {
        recorder_.record();
    }
    else
    {
        recorder_.stop();
    }
}

void InputSignal::processBuffer(QAudioBuffer buf)
{
    std::lock_guard<std::mutex> l(m_);
    FFTBuffer fft_in{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
    fft_.appendToBuff(fft_in);

    if (fft_.FFTIsReady())
    {
        auto fft_buff = fft_.getFFTBuffer();
        double biggest = std::distance(fft_buff.begin(), fft_buff.getMaxReal());
        auto w = static_cast<long double>(recorder_.NYQUIST_FREQ) / static_cast<long double>(fft_buff.size()) * static_cast<long double>(biggest) * 2;
        freq_.store(w);
        ready_.notify_all();
        fftReady.store(true);
    }
}
