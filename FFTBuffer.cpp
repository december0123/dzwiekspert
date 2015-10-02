#include "FFTBuffer.hpp"

FFTBuffer::FFTBuffer(const int NFFT)
    : data_(NFFT)
{
    for (int i = 0; i < NFFT; ++i)
    {
        data_[i].r = 0;
        data_[i].i = 0;
    }
}

FFTBuffer::FFTBuffer(const QByteArray& b)
    : data_(b.size())
{
    for (int i = 0; i < b.size(); ++i)
    {
        data_[i].r = static_cast<float>(b[i]);
        data_[i].i = 0;
    }
}

FFTBuffer::FFTBuffer(const int* data, const int size)
    : data_(size)
{
    for(int i = 0; i < size; ++i)
    {
        data_[i].r = static_cast<float>(data[i]);
        data_[i].i = 0;
    }
}

FFTBuffer::FFTBuffer(const FFTBuffer& lhs)
    : data_{lhs.data_} {}

FFTBuffer::FFTBuffer(FFTBuffer&& lhs)
    : data_{std::move(lhs.data_)} {}

void FFTBuffer::append(FFTBuffer buf)
{
    std::move(buf.begin(), buf.end(), std::back_inserter(data_));
}

void FFTBuffer::eraseNFirst(const int N)
{
    data_.erase(data_.begin(), std::next(data_.begin(), N));
}

const kiss_fft_cpx* FFTBuffer::getData() const
{
    return data_.data();
}

kiss_fft_cpx* FFTBuffer::getData()
{
    return data_.data();
}

kiss_fft_cpx& FFTBuffer::operator[](const std::size_t index)
{
    return data_[index];
}

const kiss_fft_cpx& FFTBuffer::operator[](const std::size_t index) const
{
    return data_[index];
}
