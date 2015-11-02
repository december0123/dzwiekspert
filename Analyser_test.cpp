#include "Analyser.hpp"
#include "FFTBuffer.hpp"
#include "Utils.hpp"

#include <gtest/gtest.h>

#include <QDebug>

#include <cmath>
#include <vector>
//#include <fstream>
class AnalyserFixture : public ::testing::Test
{
protected:
};

auto getMaxReal(FFTBuffer& buf)
{
    auto gowno =std::max_element(buf.begin() + 100, buf.end(),
                            [&](const auto& lhs, const auto& rhs){return lhs.r < rhs.r;});
    qDebug() << double(gowno->r);
    return gowno;
}

//TEST(Analyser, shouldPerformFFT)
//{
////    std::ofstream f{"/tmp/samples"};
//    std::vector<long double> samples;
//    constexpr Frequency FREQUENCY{440_Hz};
//    constexpr Frequency SAMPLING_FREQUENCY{16000_Hz};
//    constexpr long double PI{3.141592653589793238513L};
//    for (int i = 0; i < 10000; ++i)
//    {
//        auto sample = std::sin(2.0 * PI * i / SAMPLING_FREQUENCY * FREQUENCY);
//        samples.push_back(sample);
////        f << sample << std::endl;
//    }
//    Analyser a;
//    FFTBuffer buf(samples);
//    a.applyHannWindow(buf);
//    a.FFT(buf);
//    long double freqIndex = std::distance(buf.begin(), getMaxReal(buf));
//    auto recognizedFrequency = SAMPLING_FREQUENCY / 2.0 / buf.size() * freqIndex;
//    ASSERT_EQ(440_Hz, recognizedFrequency);
//}
