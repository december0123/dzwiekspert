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
    unsigned long long energyThreshold{0};
    for (const auto& i : buf)
    {
        energyThreshold += i.r;
    }
    energyThreshold /= buf.size();
    energyThreshold *= buf.size() / 5;
    for (unsigned long freqIndex = LOWER_BOUND_FREQ; freqIndex < UPPER_BOUND_FREQ; ++freqIndex)
    {
        // find smallest in maxima.
        auto smallestElementIndex = 0U;
        for (auto m = 0U; m != strongestNotes.size(); ++m)
        {
            if (strongestNotes[m].getFreq() < strongestNotes[smallestElementIndex].getFreq())
            {
                smallestElementIndex = m;
            }
        }

        // check if smallest is smaller than current number
        if ((strongestNotes[smallestElementIndex].getFreq() < buf[freqIndex].r))
        {
            if (buf[freqIndex].r > energyThreshold)
            {
                strongestNotes[smallestElementIndex] = Note{"", buf[freqIndex].r, static_cast<double>(freqIndex)};
            }
        }
    }
    for (Note& n : strongestNotes)
    {
        n = recognizer_.recognizeNote(recorder_.NYQUIST_FREQ / buf.size() * n.getError());
    }
    return strongestNotes;
}

Note InputSignal::getNote(const Note& idealNote)
{
    std::unique_lock<std::mutex> l(m_);
    ready_.wait(l, [&](){return fftIsReady();});
    auto strongestNotes = getNotesAndInvalidate();
    qDebug() << "IDEALNA: " << idealNote.getFullName().c_str();
    // TODO: trudnosc
    auto candidate = std::find_if(strongestNotes.begin(), strongestNotes.end(),
                                  [&](const Note& n){return n.getName() == idealNote.getName();});
    if (candidate != strongestNotes.end())
    {
        qDebug() << "Oddaje: " << candidate->getFullName().c_str();
        return *candidate;
    }
    else
    {
        std::sort(strongestNotes.begin(), strongestNotes.end(),
                  [&](const Note& lhs, const Note&rhs){return lhs.getFreq() < rhs.getFreq();});
        candidate = std::find_if(strongestNotes.begin(), strongestNotes.end(),
                                      [&](const Note& n){return n.getFreq() != 0;});
        qDebug() << "Szukam najmniejszej" << double(candidate->getFreq());
        if (candidate != strongestNotes.end())
        {
            return *candidate;
        }
        else
        {
            return Note::UNKNOWN();
        }
    }
}

void InputSignal::setBasic(const Frequency f)
{
    recognizer_.setBasic(f);
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

        {
            std::lock_guard<std::mutex> l(m_);
            internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5L);
            samplesBufferCounter_ = FFT_THRESHOLD * OVERLAP_FACTOR;
            strongestNotes_ = findStrongestNotes(tmpBuffer);
            fftReady.store(true);
        }
        ready_.notify_all();
    }
}
