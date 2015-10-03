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
        applyHann();
        outputBuff_ = run();
        buff_.eraseNFirst(buff_.size() * 0.5);
        counter_ = 15;
    }
    else
    {
        ready_ = false;
    }
}

void FFT::applyHann()
{
    constexpr double pi = 3.141592653589793238462643383279502884197169399375105820974944;
    for (unsigned int i = 0; i < buff_.size(); ++i)
    {
        buff_[i].r *= 0.5 * (1 - std::cos(2 * pi * i / (buff_.size() - 1)));
    }
}

void FFT::clear()
{
    buff_.clear();
}

FFTBuffer FFT::run()
{
    std::unique_ptr<kiss_fft_state, FreeDeleter> state{kiss_fft_alloc(buff_.size(), 0, nullptr, nullptr)};
    FFTBuffer outputBuffer(buff_.size());
    kiss_fft(state.get(), buff_.getData(), outputBuffer.getData());
    outputBuffer.eraseDataOverNyquistFreq();
    return outputBuffer;
}
