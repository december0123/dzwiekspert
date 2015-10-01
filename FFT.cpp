#include "FFT.hpp"
extern "C"
{
    #include "kissfft/kiss_fft.h"
    #include "kissfft/_kiss_fft_guts.h"
}

#include <memory>

void FFT::appendToBuff(FFTBuffer buf)
{
    buff_.append(buf);
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
    return outputBuffer;
}
