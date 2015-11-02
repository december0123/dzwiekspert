#include "InputSignal.hpp"

#include "FFTBuffer.hpp"
#include "Note.hpp"
#include "Utils.hpp"

#include <QDebug>

#include <vector>

InputSignal::InputSignal()
{
    static_assert(FFT_THRESHOLD % 2 == 0, "FFT_THRESHOLD SHOULD BE EVEN");
    probe_.setSource(&recorder_);
    QObject::connect(&probe_, &QAudioProbe::audioBufferProbed,
            this, &InputSignal::processBuffer);
}

bool InputSignal::fftIsReady() const
{
    return fftReady.load();
}

std::vector<Note> InputSignal::getNotesAndInvalidate()
{
    fftReady.store(false);
    return strongestNotes_;
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
            qDebug() << "Nie udalo sie rozpoczac nagrywania";
        }
    }
    else
    {
        recorder_.stop();
        internalBuffer_.clear();
        fftReady.store(false);
        samplesBufferCounter_ = 0;
        qDebug() << "stop";
    }
}

std::vector<Note> InputSignal::findStrongestNotes(FFTBuffer &buf) const
{
    std::vector<Note> strongestNotes(NUM_OF_STRONGEST_NOTES_TO_FIND, Note{});
    unsigned long long mean{0};
    for (const auto& i : buf)
    {
        mean += i.r;
    }
    mean /= buf.size();
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
            if (buf[freqIndex].r > mean * buf.size() / 6)
            {
                strongestNotes[smallestIndex] = Note{"", buf[freqIndex].r, static_cast<double>(freqIndex)};
            }
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

Note InputSignal::getNote(const Note& idealNote)
{
//    std::unique_lock<std::mutex> l(m_);
//    ready_.wait(l, [&](){return fftIsReady();});
    auto strongestNotes = getNotesAndInvalidate();
    auto candidate = std::find_if(strongestNotes.begin(), strongestNotes.end(),
                                  [&](const Note& n){return n.getFullName() == idealNote.getFullName();});
    if ( candidate != strongestNotes.end())
    {
        return *candidate;
    }
    else
    {
        qDebug() << "Szukam najmniejszej";
        return *std::min_element(strongestNotes.begin(), strongestNotes.end(),
                                 [&](const Note& lhs, const Note&rhs){return lhs.getFreq() < rhs.getFreq();});
    }
}

void InputSignal::processBuffer(QAudioBuffer buf)
{
    FFTBuffer fft_in{QByteArray::fromRawData(buf.constData<const char>(), buf.byteCount())};
    internalBuffer_.append(std::move(fft_in));
    if (++samplesBufferCounter_ == FFT_THRESHOLD)
    {
        FFTBuffer tmpBuffer{internalBuffer_};
        Analyser::applyHannWindow(tmpBuffer);
        Analyser::FFT(tmpBuffer);
        Analyser::HPS(tmpBuffer);
        std::lock_guard<std::mutex> l(m_);
        internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5L);
        samplesBufferCounter_ = FFT_THRESHOLD * OVERLAP_FACTOR;
        strongestNotes_ = findStrongestNotes(tmpBuffer);
        fftReady.store(true);
        ready_.notify_all();
    }
}
