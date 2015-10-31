#include "InputSignal.hpp"

#include "Utils.hpp"

#include <QDebug>
#include <fstream>

InputSignal::InputSignal()
{
    probe_.setSource(&recorder_);
    QObject::connect(&probe_, &QAudioProbe::audioBufferProbed,
            this, &InputSignal::processBuffer);

    noiseProbe_.setSource(&noiseRecorder_);
    QObject::connect(&noiseProbe_, &QAudioProbe::audioBufferProbed,
            this, &InputSignal::processNoise);
}

bool InputSignal::fftIsReady() const
{
    return fftReady.load();
}

Note trivial(std::deque<Note>& src)
{
    int count = 0;
    Note majorityElement; // will be assigned before use for valid args

    for (int i = 0; i < src.size(); ++i)
    {
        if (count == 0)
            majorityElement = src[i];

        if (src[i].getName() == majorityElement.getName())
           ++count;
        else
           --count;
    }

    count = 0;
    for (int i = 0; i < src.size(); ++i)
    {
        if (src[i].getName() == majorityElement.getName())
            count++;
    }

    if (3 * count >= src.size())
        return majorityElement;
    else
        throw "";
}

Note InputSignal::getNoteAndInvalidate()
{
    fftReady.store(false);
    lastNotes_.push_back(note_);
    if (lastNotes_.size() > 20)
    {
        lastNotes_.pop_front();
        try
        {
            return trivial(lastNotes_);
        }
        catch(...)
        {
            auto qwe = *std::min_element(lastNotes_.begin(), lastNotes_.end(),
                                     [&](const Note& n1, const Note& n2){return n1.getFreq() < n2.getFreq();});
            return qwe;
        }
    }
    return note_;
}

void InputSignal::capture(bool capture)
{
    if (capture)
    {
//        recordNoise();
        if (recorder_.isAvailable())
        {
            qDebug() << "nagralo halas";

            recorder_.record();
            qDebug() << "Nagrywa";
        }
        else
        {
            qDebug() << "jeblo";
        }
    }
    else
    {
        qDebug() << "Konczy nagrywanie";
        recorder_.stop();
        qDebug() << "Konczy nagrywanie2";
        internalBuffer_.clear();
        noise_.clear();
        fftReady.store(false);
        samplesBufferCounter_ = 0;
        qDebug() << "Recorder stop segfault";
    }
}

std::ofstream f{"/tmp/samples"};
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
//        qDebug() << samplesBufferCounter_ << tmpBuffer.size() << noise_.size();
//        for(unsigned long i = 0; i < tmpBuffer.size(); ++i)
//        {
//            tmpBuffer[i].r -= noise_[i].r;
//            tmpBuffer[i].i -= noise_[i].i;
//        }

//        analyser_.HPS(tmpBuffer);
        internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5L);
        samplesBufferCounter_ = FFT_THRESHOLD * OVERLAP_FACTOR;
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
//        qDebug() << "fftReady";
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
//        qDebug() << "no witam";

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
//        qDebug() << "u foken wot" << noise_.size() << noise.size();
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
