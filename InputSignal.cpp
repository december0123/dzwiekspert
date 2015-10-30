#include "InputSignal.hpp"

#include <QDebug>

InputSignal::InputSignal()
{
    probe_.setSource(&recorder_);

}

bool InputSignal::fftIsReady() const
{
    return fftReady.load();
}

Note InputSignal::getNoteAndInvalidate()
{
    fftReady.store(false);
    return note_;
}

void InputSignal::capture(bool capture)
{
    if (capture && recorder_.isAvailable())
    {
        QObject::connect(&probe_, &QAudioProbe::audioBufferProbed,
                this, &InputSignal::processBuffer);
        recorder_.record();
    }
    else
    {
        recorder_.stop();
        internalBuffer_.clear();
        outputBuff_.clear();
        samplesBufferCounter_ = 0;
        qDebug() << "Recorder stop segfault";
        QObject::disconnect(&probe_, &QAudioProbe::audioBufferProbed,
                this, &InputSignal::processBuffer);
    }
}

void InputSignal::processBuffer(QAudioBuffer buf)
{
    FFTBuffer fft_in{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
    internalBuffer_.append(std::move(fft_in));
    if (++samplesBufferCounter_ == FFT_THRESHOLD)
    {
        FFTBuffer tmpBuffer{internalBuffer_};
        analyser_.applyHannWindow(tmpBuffer);
        analyser_.HPS(tmpBuffer);
        outputBuff_ = std::move(tmpBuffer);
        internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5L);
        samplesBufferCounter_ = OVERLAP_FACTOR;
        auto max = getMaxReal(outputBuff_);
        long double biggest = std::distance(outputBuff_.begin(), max);
        auto w = recorder_.NYQUIST_FREQ / outputBuff_.size() * biggest;
        try
        {
            note_ = s_.recognizeNote(w);
        }
        catch(const std::logic_error& e)
        {
            qDebug() << e.what();
        }

        fftReady.store(true);
        ready_.notify_all();
    }
}
