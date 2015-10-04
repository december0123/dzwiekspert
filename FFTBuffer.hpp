#ifndef FFTBUFFER_HPP
#define FFTBUFFER_HPP

extern "C" {
    #include "kissfft/kiss_fft.h"
}

#include <QByteArray>
#include <vector>

class FFTBuffer
{
public:
    explicit FFTBuffer(const int NFFT);
    FFTBuffer(const QByteArray& b);
    FFTBuffer(const FFTBuffer& lhs);
    FFTBuffer(FFTBuffer&& lhs);
    FFTBuffer() = default;
    ~FFTBuffer() = default;
    FFTBuffer operator=(const FFTBuffer& lhs);
    FFTBuffer operator=(FFTBuffer&& lhs);

    void append(FFTBuffer buf);
    void eraseNFirst(const int N);

    const kiss_fft_cpx* getData() const;
    kiss_fft_cpx* getData();
    kiss_fft_cpx& operator[](const std::size_t index);
    const kiss_fft_cpx& operator[](const std::size_t index) const;
    void eraseDataOverNyquistFreq();

    auto getMaxReal()
    {
        return std::max_element(std::next(data_.begin(), LOWER_BOUND_FREQ), std::next(data_.begin(), UPPER_BOUND_FREQ),
                                [&](const auto& lhs, const auto& rhs){return lhs.r < rhs.r;});
    }
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
    const int LOWER_BOUND_FREQ{20};
    const int UPPER_BOUND_FREQ{100};
};


#endif // FFTBUFFER_HPP
