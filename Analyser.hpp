#ifndef ANALYSER_HPP
#define ANALYSER_HPP

#include "FFTBuffer.hpp"

#include <atomic>

struct FreeDeleter
{
    void operator()(void* p) {free(p);}
};

class Analyser
{
public:
    void appendToBuff(FFTBuffer buf);
    void clear();
    void FFT(FFTBuffer& input);
    void IFFT(FFTBuffer& input);
    void HPS(FFTBuffer& input);
    void autoCor(FFTBuffer& input);
    FFTBuffer getFFTBuffer();
    bool FFTIsReady() const;
    double OUT_FREQ{0};

private:
    FFTBuffer outputBuff_;
    FFTBuffer internalBuffer_;
    unsigned samplesBufferCounter_ = 0;
    const unsigned FFT_THRESHOLD = 40;
    const unsigned OVERLAP_FACTOR = FFT_THRESHOLD / 2;
    std::atomic<bool> ready_{false};

    void applyHannWindow(FFTBuffer& input);
};

#endif // ANALYSER_HPP
