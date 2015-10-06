#ifndef FFTBUFFER_HPP
#define FFTBUFFER_HPP

extern "C" {
    #include "kissfft/kiss_fft.h"
}

#include <QByteArray>
#include <vector>
#include <QDebug>

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

    auto autoCor() const
    {
        std::vector<double> R(data_.size());
        long double sum{0.0L};
        for (unsigned long long delta = 0; delta < data_.size(); ++delta)
        {
            sum = 0.0L;
            for (unsigned long long index = 0; index < data_.size() - delta; ++index)
            {
                sum += data_[index].r * data_[index+delta].r;
            }
            R[delta] = sum;
        }
        auto max = 1.0 / ( std::distance(R.begin(), std::max_element(std::next(R.begin(), 50), R.end())) / 16000.0);
//        auto witam2 = 1.0 / ( getLocalMaxIndex(R) / 16000.0);
        return max;
    }

    double getLocalMaxIndex(const std::vector<double>& v) const
    {
        for (std::size_t i = 1000; i < v.size() - 1; ++i)
        {
            if(v[i - 1] < v[i] && v[i] > v[i + 1])
            {
                return v[i];
            }
        }
        return 0.0;
    }

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
    const int LOWER_BOUND_FREQ{7};
    const int UPPER_BOUND_FREQ{100};
};


#endif // FFTBUFFER_HPP
