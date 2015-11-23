#include "Analyzer.hpp"

extern "C"
{
    #include "kissfft/kiss_fft.h"
    #include "kissfft/_kiss_fft_guts.h"
}

#include <QDebug>
#include <cmath>
#include <memory>
#include <Utils.hpp>

// Fast Fourier Transform
void Analyzer::FFT(FFTBuffer& input)
{
    std::unique_ptr<kiss_fft_state, FreeDeleter> state{kiss_fft_alloc(input.size(), 0, nullptr, nullptr)};
    FFTBuffer fft(input.size());
    kiss_fft(state.get(), input.getData(), fft.getData());
    fft.eraseDataOverNyquistFreq();

    for (auto& bin : fft)
    {
        bin.r = std::abs(bin.r * bin.i);
    }
    input = std::move(fft);
}

// Harmonic Product Spectrum
void Analyzer::HPS(FFTBuffer &input)
{
    FFTBuffer hps{input};
    for (int factor = 1; factor <= DOWNSAMPLING_FACTOR; ++factor)
    {
        // Compute HPS for each sample of the downsampled signal
        for (unsigned long sample = 0; sample < input.size() / factor; ++sample)
        {
            hps[sample].r *= input[sample * factor].r;
        }
    }
    input = std::move(hps);
}

void Analyzer::applyHannWindow(FFTBuffer& input)
{
    for (long double i = 0; i < input.size(); ++i)
    {
        input[i].r *= 0.5L * (1.0L - std::cos(2.0L * PI<long double> * i / (input.size() - 1)));
    }
}
