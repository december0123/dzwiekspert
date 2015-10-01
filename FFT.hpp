#ifndef FFT_HPP
#define FFT_HPP

#include "FFTBuffer.hpp"

struct FreeDeleter
{
    void operator()(void* p) {free(p);}
};

class FFT
{
public:
    static FFTBuffer run(const int NFFT, const FFTBuffer& inputBuffer);
};

#endif // FFT_HPP
