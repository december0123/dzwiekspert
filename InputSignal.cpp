#include "InputSignal.hpp"
#include <QDebug>

InputSignal::InputSignal()
{
    probe_.setSource(&audioRecorder);
    QObject::connect(&probe_, &QAudioProbe::audioBufferProbed,
            this, &InputSignal::processBuffer);
}

int InputSignal::getFreq() const
{
    qDebug() << "Oddalem " << freq_.load();
    return freq_.load();
}

void InputSignal::capture(bool capture)
{
    contCapture_.store(capture);
    if (capture && audioRecorder.isAvailable())
    {
        audioRecorder.record();
    }
    else
    {
        audioRecorder.stop();
    }
}

void InputSignal::processBuffer(QAudioBuffer buf)
{
    std::lock_guard<std::mutex> l(m_);
    FFTBuffer fft_in{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
    fft_.appendToBuff(fft_in);

    if (fft_.ready_)
    {
        auto fft_buff = fft_.outputBuff_;
        double biggest = std::distance(fft_buff.begin(), fft_buff.getMaxReal());
        auto w = static_cast<long double>(audioRecorder.nyquistFreq) / static_cast<long double>(fft_buff.size()) * static_cast<long double>(biggest) * 2;
        freq_.store(w);
        ready.notify_all();
        fftReady = true;
        fft_.ready_ = false;
    }
}
