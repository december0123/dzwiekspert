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
        fftReady.store(false);
        samplesBufferCounter_ = 0;
    }
}

std::vector<Note> InputSignal::findStrongestNotes(FFTBuffer &buf) const
{
    std::vector<Note> strongestNotes(5, Note{});
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
        if ((strongestNotes[smallestIndex].getFreq() < buf[freqIndex].r))
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
        qDebug() << QString::fromStdString(i.getFullName());
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
        fftReady.store(true);
        ready_.notify_all();
    }
}
