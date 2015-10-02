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
    FFTBuffer run();
    void appendToBuff(FFTBuffer buf);
    void clear();
private:
    unsigned counter_ = 0;
    FFTBuffer buff_;
};

#endif // FFT_HPP
