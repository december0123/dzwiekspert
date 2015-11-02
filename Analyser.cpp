#include "Analyser.hpp"

extern "C"
{
    #include "kissfft/kiss_fft.h"
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
        i.r = std::abs(i.r * i.i);
    }
    input = std::move(fft);
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

void Analyser::applyHannWindow(FFTBuffer& input)
{
    constexpr long double PI{3.141592653589793238513L};

    for (long double i = 0; i < input.size(); ++i)
    {
        input[i].r *= 0.5L * (1.0L - std::cos(2.0L * PI * i / (input.size() - 1)));
    }
}
