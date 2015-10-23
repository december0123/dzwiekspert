#include "Analyser.hpp"
extern "C"
{
    #include "kissfft/kiss_fft.h"
    #include "kissfft/_kiss_fft_guts.h"
}

#include <string>
#include <cmath>
#include <QDebug>
#include <memory>

void Analyser::appendToBuff(FFTBuffer buf)
{
    internalBuffer_.append(std::move(buf));
    if (++samplesBufferCounter_ == FFT_THRESHOLD)
    {
        FFTBuffer tmpBuffer{internalBuffer_};
        applyHannWindow(tmpBuffer);
        HPS(tmpBuffer);
        outputBuff_ = std::move(tmpBuffer);
        internalBuffer_.eraseNFirst(internalBuffer_.size() * 0.5);
        ready_.store(true);
        samplesBufferCounter_ = OVERLAP_FACTOR;
    }
    else
    {
        ready_.store(false);
    }
}

void Analyser::clear()
{
    internalBuffer_.clear();
}

// Fast Fourier Transform
void Analyser::FFT(FFTBuffer& input)
{
    std::unique_ptr<kiss_fft_state, FreeDeleter> state{kiss_fft_alloc(input.size(), 0, nullptr, nullptr)};
//    FFTBuffer fft(input.size());
    kiss_fft(state.get(), input.getData(), input.getData());
    input.eraseDataOverNyquistFreq();
    for (auto& i : input)
    {
        i.r = std::abs(i.r);
    }
}

// Harmonic Product Spectrum
void Analyser::HPS(FFTBuffer &input)
{
    FFT(input);
    FFTBuffer hps{input};
    // Go through each downsampling factor
    constexpr int N = 20;
    for (int downsamplingFactor = 1; downsamplingFactor <= N; ++downsamplingFactor)
    {
        // Go through samples of the downsampled signal and compute HPS at this iteration
        for (int idx = 0; idx < input.size() / downsamplingFactor; ++idx)
        {
            hps[idx].r *= input[idx * downsamplingFactor].r;
        }
    }
    input = std::move(hps);
}

FFTBuffer Analyser::getFFTBuffer()
{
    ready_.store(false);
    return std::move(outputBuff_);
}

bool Analyser::FFTIsReady() const
{
    return ready_.load();
}

void Analyser::applyHannWindow(FFTBuffer& b)
{
    constexpr long double pi{3.141592653589793238513L};

    for (long double i = 0; i < b.size(); ++i)
    {
        b[i].r *= 0.5L * (1.0L - std::cos(2.0L * pi * i / (b.size() - 1)));
    }
}
