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
    FFTBuffer(const int NFFT);
    FFTBuffer(const QByteArray& b);
    FFTBuffer(const int* data, const int size);
    FFTBuffer(const FFTBuffer& lhs);
    FFTBuffer(FFTBuffer&& lhs);
    FFTBuffer() = default;
    ~FFTBuffer() = default;

    void append(FFTBuffer buf);


    const kiss_fft_cpx* getData() const;
    kiss_fft_cpx* getData();
    kiss_fft_cpx& operator[](const std::size_t index);
    const kiss_fft_cpx& operator[](const std::size_t index) const;
    void push_back(kiss_fft_cpx item);

    auto getMaxReal()
    {
        return std::max_element(data_.begin(), data_.end(),
                                [&](const auto& lhs, const auto& rhs){return lhs.r < rhs.r;});
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
    auto eraseDataOverNyquistFreq()
    {
        data_.erase(data_.begin(), std::next(data_.begin(), data_.size() / 2));
        data_.erase(std::next(data_.begin(), data_.size() / 2), data_.end());
    }

    auto eraseNFirst(const int N)
    {
        return data_.erase(data_.begin(), std::next(data_.begin(), N));
    }

private:
    std::vector<kiss_fft_cpx> data_;
};


#endif // FFTBUFFER_HPP
