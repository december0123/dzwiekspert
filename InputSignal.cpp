#include "InputSignal.hpp"

#include "Utils.hpp"

#include <QDebug>
#include <fstream>

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
    fftReady.store(false);
    return note_;
}

void InputSignal::capture(bool capture)
{
    if (capture)
    {
        if (recorder_.isAvailable())
        {
            recorder_.record();
        }
        else
        {
            qDebug() << "jeblo";
        }
    }
    else
    {
        recorder_.stop();
        internalBuffer_.clear();
        noise_.clear();
        fftReady.store(false);
        samplesBufferCounter_ = 0;
    }
}

std::vector<Note> InputSignal::findStrongestNotes(FFTBuffer &buf) const
{
    std::vector<Note> strongestNotes(3, Note{});
    for (unsigned long freqIndex = LOWER_BOUND_FREQ; freqIndex < UPPER_BOUND_FREQ; ++freqIndex)
    {
        // find smallest in maxima.
        auto smallestIndex = 0U;
        for (auto m = 0U; m != strongestNotes.size(); ++m)
        {
            if (strongestNotes[m].getFreq() < strongestNotes[smallestIndex].getFreq())
            {
                smallestIndex = m;
            }
        }

        // check if smallest is smaller than current number
        if (strongestNotes[smallestIndex].getFreq() < buf[freqIndex].r && buf[freqIndex].r > 10000)
        {
            strongestNotes[smallestIndex] = Note{"", buf[freqIndex].r, static_cast<double>(freqIndex)};
        }
    }
    for (Note& n : strongestNotes)
    {
        n = s_.recognizeNote(recorder_.NYQUIST_FREQ / buf.size() * n.getError());
    }
    for (const Note& i : strongestNotes_)
    {
        qDebug() << QString::fromStdString(i.getName());
    }
    return strongestNotes;
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
        analyser_.FFT(tmpBuffer);
        analyser_.HPS(tmpBuffer);
        internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5L);
        samplesBufferCounter_ = FFT_THRESHOLD * OVERLAP_FACTOR;
        strongestNotes_ = findStrongestNotes(tmpBuffer);
        long double freqIndex = std::distance(tmpBuffer.begin(), getSecondMaxReal(tmpBuffer));
        auto recognizedFrequency = recorder_.NYQUIST_FREQ / tmpBuffer.size() * freqIndex;
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

void InputSignal::recordNoise()
{
    if (noiseRecorder_.isAvailable())
    {
        noiseRecorder_.record();
        std::unique_lock<std::mutex> l(m_);

        ready_.wait(l, [&](){return fftIsReady();});
        fftReady.store(false);
        qDebug() << "Nagralo halas";
    }
}

void InputSignal::processNoise(QAudioBuffer buf)
{
    static unsigned i = 0;
    if (i > FFT_THRESHOLD * 3)
    {
        FFTBuffer noise{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
        noise_.append(std::move(noise));
        if (++samplesBufferCounter_ == FFT_THRESHOLD)
        {
            noiseRecorder_.stop();
            analyser_.applyHannWindow(noise_);
            analyser_.FFT(noise_);
            long double freqIndex = std::distance(noise_.begin(), getMaxReal(noise_));
            auto recognizedFrequency = recorder_.NYQUIST_FREQ / noise_.size() * freqIndex;
            try
            {
                qDebug() << s_.recognizeNote(recognizedFrequency).getName().c_str() << double(recognizedFrequency);
            }
            catch(const std::logic_error& e)
            {
                qDebug() << e.what();
            }
            samplesBufferCounter_ = 0;
            fftReady.store(true);
            ready_.notify_all();
        }
    }
    else
        ++i;
}
