#include "FFT.hpp"
extern "C"
{
    #include "kissfft/kiss_fft.h"
    #include "kissfft/_kiss_fft_guts.h"
}

#include <string>
#include <cmath>
#include <QDebug>
#include <memory>

void FFT::appendToBuff(FFTBuffer buf)
{
    internalBuffer_.append(buf);
    if (++samplesBufferCounter_ == FFT_THRESHOLD)
    {
        FFTBuffer tmpBuffer{internalBuffer_};
        applyHannWindow(tmpBuffer);
        outputBuff_ = run(tmpBuffer);
//        OUT_FREQ = tmpBuffer.autoCor();
//        qDebug() << "LISU PLS: " << OUT_FREQ;
        // overlap factor 50%
        internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5);
        ready_.store(true);
        samplesBufferCounter_ = OVERLAP_FACTOR;
    }
    else
    {
        ready_.store(false);
    }
}

void FFT::clear()
{
    internalBuffer_.clear();
}

FFTBuffer FFT::run(const FFTBuffer& input)
{
    std::unique_ptr<kiss_fft_state, FreeDeleter> state{kiss_fft_alloc(input.size(), 0, nullptr, nullptr)};
    FFTBuffer outputBuffer(input.size());
    kiss_fft(state.get(), input.getData(), outputBuffer.getData());
    outputBuffer.eraseDataOverNyquistFreq();
    for (auto& i : outputBuffer)
    {
        i.r = std::abs(i.r);
    }
    return outputBuffer;
}

FFTBuffer FFT::getFFTBuffer()
{
    ready_.store(false);
    FFTBuffer b = std::move(outputBuff_);
    return b;
}

bool FFT::FFTIsReady() const
{
    return ready_.load();
}

void FFT::applyHannWindow(FFTBuffer& b)
{
    constexpr long double pi{3.141592653589793238513L};

    for (long double i = 0; i < b.size(); ++i)
    {
        b[i].r *= 0.5L * (1.0L - std::cos(2.0L * pi * i / (b.size() - 1)));
    }
}
