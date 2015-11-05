#include "Analyser.hpp"

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

    for (int factor = 1; factor <= DOWNSAMPLING_FACTOR; ++factor)
    {
        // Go through samples of the downsampled signal and compute HPS at this iteration
        for (unsigned long idx = 0; idx < input.size() / factor; ++idx)
        {
            hps[idx].r *= input[idx * factor].r;
        }
    }
    input = std::move(hps);
}

void Analyser::applyHannWindow(FFTBuffer& input)
{


    for (long double i = 0; i < input.size(); ++i)
    {
        input[i].r *= 0.5L * (1.0L - std::cos(2.0L * PI<long double> * i / (input.size() - 1)));
    }
}
