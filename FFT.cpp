#include "FFT.hpp"
extern "C"
{
    #include "kissfft/kiss_fft.h"
    #include "kissfft/_kiss_fft_guts.h"
}

#include <cmath>
#include <memory>
#include <QDebug>

void FFT::appendToBuff(FFTBuffer buf)
{
    buff_.append(buf);
    if (++counter_ == 30)
    {
        ready_ = true;
        FFTBuffer b{buff_};
//        substractAvg(b);
        applyHann(b);
        outputBuff_ = run(b);
        buff_.eraseNFirst(buff_.size() * 0.5);
        counter_ = 15;
    }
    else
    {
        ready_ = false;
    }
}

void FFT::substractAvg(FFTBuffer& b)
{
    long acc{0};
    for (const auto& i : b)
    {
        acc += i.r;
    }
    acc /= b.size();
    for (auto& i : b)
    {
        i.r -= acc;
    }
}

void FFT::applyHann(FFTBuffer& b)
{
    constexpr long double pi = 3.141592653589793238462643383279502884197169399375105820974944;
    for (long double i = 0; i < b.size(); ++i)
    {
        b[i].r *= 0.5 * (1.0L - std::cos(2.0L * pi * i / (b.size() - 1)));
    }
}

void FFT::clear()
{
    buff_.clear();
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
