#ifndef ANALYSER_HPP
#define ANALYSER_HPP

#include "FFTBuffer.hpp"

struct FreeDeleter
{
    void operator()(void* p) {free(p);}
};

class Analyser
{
public:
    Analyser() = delete;
    static void applyHannWindow(FFTBuffer& input);
    static void FFT(FFTBuffer& input);
    static void IFFT(FFTBuffer& input);
    static void HPS(FFTBuffer& input);
    static void autoCor(FFTBuffer& input);
};

#endif // ANALYSER_HPP
