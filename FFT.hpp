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
    FFTBuffer outputBuff_;
    bool ready_ = false;
private:
    unsigned counter_ = 0;
    FFTBuffer buff_;

    void applyHann();
};

#endif // FFT_HPP
