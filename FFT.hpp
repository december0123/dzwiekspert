#ifndef FFT_HPP
#define FFT_HPP

extern "C" {
    #include "kissfft/kissfft/kiss_fft.h"
    #include "kissfft/kissfft/_kiss_fft_guts.h"
}

#include <vector>
#include <QDebug>
#include <QByteArray>

class FFTBuffer
{
public:
    FFTBuffer(const QByteArray& b)
        : data_(b.size())
    {
        for (int i = 0; i < b.size(); ++i)
        {
            data_[i].r = static_cast<float>(b[i]);
            data_[i].i = 0;
        }
    }

    FFTBuffer(const int NFFT)
        : data_(NFFT)
    {
        for (int i = 0; i < NFFT; ++i)
        {
            data_[i].r = 0;
            data_[i].i = 0;
        }
    }

    FFTBuffer(const FFTBuffer& lhs)
        : data_(lhs.data_) {}

    FFTBuffer(FFTBuffer&& lhs)
        : data_{std::move(lhs.data_)} {}

    FFTBuffer(const int* data, const int size)
        : data_(size)
    {
        for(int i = 0; i < size; ++i)
        {
            data_[i].r = data[i];
            data_[i].i = 0;
        }
    }

    FFTBuffer() = delete;
    ~FFTBuffer() = default;

    const kiss_fft_cpx* getData() const
    {
        return data_.data();
    }

    kiss_fft_cpx* getData()
    {
        return data_.data();
    }

    kiss_fft_cpx& operator[](const std::size_t index)
    {
        return data_[index];
    }

    const kiss_fft_cpx& operator[](const std::size_t index) const
    {
        return data_[index];
    }

    void push_back(kiss_fft_cpx item)
    {
        data_.push_back(item);
    }

    auto size() const
    {
        return data_.size();
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
