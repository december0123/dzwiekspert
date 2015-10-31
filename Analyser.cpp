#include "Analyser.hpp"

extern "C"
{
    #include "kissfft/kiss_fft.h"
    #include "kissfft/kiss_fftr.h"
    #include "kissfft/_kiss_fft_guts.h"
}

#include <QDebug>
#include <cmath>
#include <memory>

// Fast Fourier Transform
void Analyser::FFT(FFTBuffer& input)
{
    std::unique_ptr<kiss_fft_state, FreeDeleter> state{kiss_fft_alloc(input.size(), 0, nullptr, nullptr)};
    FFTBuffer fft(input.size());
    kiss_fft(state.get(), input.getData(), fft.getData());
    fft.eraseDataOverNyquistFreq();

    for (auto& i : fft)
    {
        i.r = std::abs(i.r);
    }
    input = std::move(fft);
}

void Analyser::IFFT(FFTBuffer &input)
{
    std::unique_ptr<kiss_fftr_state, FreeDeleter> state{kiss_fftr_alloc(input.size(), 0, nullptr, nullptr)};
    FFTBuffer fft(input.size());
    std::vector<kiss_fft_scalar> ifft(input.size());
    kiss_fftri(state.get(), input.getData(), ifft.data());
    ifft.shrink_to_fit();
    input = std::move(ifft);
}

// Harmonic Product Spectrum
void Analyser::HPS(FFTBuffer &input)
{
    FFTBuffer hps{input};
    // Go through each downsampling factor
    constexpr int N = 1;
    for (int downsamplingFactor = 1; downsamplingFactor <= N; ++downsamplingFactor)
    {
        // Go through samples of the downsampled signal and compute HPS at this iteration
        for (unsigned long idx = 0; idx < input.size() / downsamplingFactor; ++idx)
        {
            hps[idx].r *= input[idx * downsamplingFactor].r;
        }
    }
    input = std::move(hps);
}

void Analyser::autoCor(FFTBuffer &input)
{
//    FFT(input);
//    IFFT(input);
    std::vector<long double> R(input.size());

    long double sum{0.0L};
    for (unsigned long long delta = 0; delta < input.size(); ++delta)
    {
        sum = 0.0L;
        for (unsigned long long index = 0; index < input.size() - delta; ++index)
        {
            sum += input[index].r * input[index+delta].r;
        }
        R[delta] = sum;
    }
    FFTBuffer fft{R};

    FFT(fft);
    input = std::move(fft);
}

void Analyser::applyHannWindow(FFTBuffer& input)
{
    constexpr long double pi{3.141592653589793238513L};

    for (long double i = 0; i < input.size(); ++i)
    {
        input[i].r *= 0.5L * (1.0L - std::cos(2.0L * pi * i / (input.size() - 1)));
    }
}
