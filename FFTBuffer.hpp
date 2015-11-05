#ifndef FFTBUFFER_HPP
#define FFTBUFFER_HPP

extern "C" {
    #include "kissfft/kiss_fft.h"
}

#include <QByteArray>
#include <QDebug>

#include <vector>

class FFTBuffer
{
public:
    explicit FFTBuffer(const int NFFT);
    FFTBuffer(const QByteArray& b);
    FFTBuffer(const FFTBuffer& lhs);
    FFTBuffer(FFTBuffer&& lhs);
    FFTBuffer(const std::vector<long double> &rhs);
    FFTBuffer() = default;
    ~FFTBuffer() = default;
    FFTBuffer& operator=(const FFTBuffer& lhs);
    FFTBuffer& operator=(FFTBuffer&& lhs);

    void append(FFTBuffer buf);
    void eraseNFirst(const int N);

    const kiss_fft_cpx* getData() const;
    kiss_fft_cpx* getData();
    kiss_fft_cpx& operator[](const std::size_t index);
    const kiss_fft_cpx& operator[](const std::size_t index) const;
    void eraseDataOverNyquistFreq();

    auto capacity() const
    {
        return data_.capacity();
    }
    auto size() const
    {
        return data_.size();
    }
    auto clear()
    {
        return data_.clear();
    }
    auto begin()
    {
        return data_.begin();
    }
    auto end()
    {
        return data_.end();
    }
    auto cbegin()
    {
        return data_.cbegin();
    }
    auto cend()
    {
        return data_.cend();
    }

private:
    std::vector<kiss_fft_cpx> data_;
};

#endif // FFTBUFFER_HPP
