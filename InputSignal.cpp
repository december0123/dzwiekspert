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
    static_assert(FFT_THRESHOLD % 2 == 0, "FFT_THRESHOLD SHOULD BE EVEN");
    FFTBuffer fft_in{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
    internalBuffer_.append(std::move(fft_in));
    if (++samplesBufferCounter_ == FFT_THRESHOLD)
    {
        FFTBuffer tmpBuffer{internalBuffer_};
        analyser_.applyHannWindow(tmpBuffer);
        analyser_.HPS(tmpBuffer);
        outputBuff_ = std::move(tmpBuffer);
        internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5L);
        samplesBufferCounter_ = FFT_THRESHOLD * OVERLAP_FACTOR;
        long double freqIndex = std::distance(outputBuff_.begin(), getMaxReal(outputBuff_));
        auto recognizedFrequency = recorder_.NYQUIST_FREQ / outputBuff_.size() * freqIndex;
        try
        {
            note_ = s_.recognizeNote(recognizedFrequency);
        }
        catch(const std::logic_error& e)
        {
            qDebug() << e.what();
        }

        fftReady.store(true);
        ready_.notify_all();
    }
}
