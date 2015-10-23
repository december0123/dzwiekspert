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

int InputSignal::getFreqAndInvalidate()
{
    qDebug() << "Oddalem " << freq_.load();
    fftReady.store(false);
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
        analyser_.clear();
    }
}

void InputSignal::processBuffer(QAudioBuffer buf)
{
//    std::lock_guard<std::mutex> l(m_);
    FFTBuffer fft_in{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
    analyser_.appendToBuff(fft_in);

    if (analyser_.FFTIsReady())
    {
        auto fft_buff = analyser_.getFFTBuffer();
        auto max = getMaxReal(fft_buff);
        long double biggest = std::distance(fft_buff.begin(), max);
        auto w = static_cast<long double>(recorder_.NYQUIST_FREQ) / static_cast<long double>(fft_buff.size()) * biggest;
//        qDebug() << "Freq: " << (double)w << "Moc: " << (double)max->r;
        freq_.store(w);
//        freq_.store(fft_.OUT_FREQ);
        ready_.notify_all();
        fftReady.store(true);
    }
}
