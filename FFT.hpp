#ifndef FFT_HPP
#define FFT_HPP

#include "FFTBuffer.hpp"

#include <atomic>

struct FreeDeleter
{
    void operator()(void* p) {free(p);}
};

class FFT
{
public:
    void appendToBuff(FFTBuffer buf);
    void clear();
    FFTBuffer run(const FFTBuffer &input);
    FFTBuffer getFFTBuffer();
    bool FFTIsReady() const;

private:
    FFTBuffer outputBuff_;
    FFTBuffer internalBuffer_;
    unsigned samplesBufferCounter_ = 0;
    const unsigned FFT_THRESHOLD = 40;
    const unsigned OVERLAP_FACTOR = FFT_THRESHOLD / 2;
    std::atomic<bool> ready_{false};

    void applyHannWindow(FFTBuffer& b);
};

#endif // FFT_HPP
