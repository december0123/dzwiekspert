#include "FFTBuffer.hpp"

FFTBuffer::FFTBuffer(const int NFFT)
    : data_(NFFT)
{
    for (int i = 0; i < NFFT; ++i)
    {
        data_[i].r = 0.0L;
        data_[i].i = 0.0L;
    }
}

FFTBuffer::FFTBuffer(const QByteArray& b)
    : data_(b.size())
{
    for (int i = 0; i < b.size(); ++i)
    {
        data_[i].r = static_cast<long double>(b[i]);
        data_[i].i = 0.0L;
    }
}

FFTBuffer::FFTBuffer(const FFTBuffer& lhs)
    : data_{lhs.data_} {}

FFTBuffer::FFTBuffer(FFTBuffer&& lhs)
    : data_{std::move(lhs.data_)} {}

FFTBuffer FFTBuffer::operator=(const FFTBuffer& lhs)
{
    data_ = lhs.data_;
    return *this;
}

FFTBuffer FFTBuffer::operator=(FFTBuffer&& lhs)
{
    data_ = std::move(lhs.data_);
    return *this;
}

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

void FFTBuffer::eraseDataOverNyquistFreq()
{
    data_.erase(data_.begin(), std::next(data_.begin(), data_.size() / 2));
    data_.erase(std::next(data_.begin(), data_.size() / 2), data_.end());
}
