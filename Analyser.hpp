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
    void applyHannWindow(FFTBuffer& input);
    void FFT(FFTBuffer& input);
    void IFFT(FFTBuffer& input);
    void HPS(FFTBuffer& input);
    void autoCor(FFTBuffer& input);
};

#endif // ANALYSER_HPP
