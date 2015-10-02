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
    void eraseNFirst(const int N);

    const kiss_fft_cpx* getData() const;
    kiss_fft_cpx* getData();
    kiss_fft_cpx& operator[](const std::size_t index);
    const kiss_fft_cpx& operator[](const std::size_t index) const;

    auto getMaxReal()
    {
        return std::max_element(std::next(data_.begin(), 20), std::next(data_.begin(), 100),
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
    auto push_back(kiss_fft_cpx item)
    {
        return data_.push_back(item);
    }

private:
    std::vector<kiss_fft_cpx> data_;
};


#endif // FFTBUFFER_HPP
