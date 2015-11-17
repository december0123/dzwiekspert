#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include "FFTBuffer.hpp"

struct FreeDeleter
{
    void operator()(void* p) {free(p);}
};

class Analyzer
{
public:
    Analyzer() = delete;
    static void applyHannWindow(FFTBuffer& input);
    static void FFT(FFTBuffer& input);
    static void IFFT(FFTBuffer& input);
    static void HPS(FFTBuffer& input);

private:
    constexpr static int DOWNSAMPLING_FACTOR{1};
};

#endif // ANALYZER_HPP
