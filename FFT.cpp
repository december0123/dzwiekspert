#include "FFT.hpp"
extern "C"
{
    #include "kissfft/kiss_fft.h"
    #include "kissfft/_kiss_fft_guts.h"
}

#include <memory>
#include <QDebug>

void FFT::appendToBuff(FFTBuffer buf)
{
    ++counter_;
    buff_.append(buf);
    if (counter_ == 10)
    {
        ready_ = true;
        outputBuff_ = std::move(run());
        buff_.eraseNFirst(buff_.size() * 0.5);
        counter_ = 5;
    }
    else
    {
        ready_ = false;
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
    qDebug() << outputBuffer.size();
    return outputBuffer;
}
