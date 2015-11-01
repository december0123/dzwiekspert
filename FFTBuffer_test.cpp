#include "FFTBuffer.hpp"

#include <gtest/gtest.h>

#include <vector>

class FFTBufferFixture : public ::testing::Test
{
protected:
    constexpr static unsigned SIZE{3U};
};

TEST_F(FFTBufferFixture, shouldBeAbleToAppend)
{
    FFTBuffer buf;
    ASSERT_EQ(0, buf.size());
    FFTBuffer newBuf(SIZE);
    buf.append(newBuf);
    // FIXME jaja sa z constexprem chyba
    ASSERT_EQ(3, buf.size());
}

TEST_F(FFTBufferFixture, shouldBeAbleToErase)
{
    std::vector<kiss_fft_cpx> testElements(SIZE);
    FFTBuffer buf(SIZE);
    for (auto i = 0U; i < SIZE; ++i)
    {
        testElements[i] = {static_cast<long double>(i), static_cast<long double>(i)};
        buf[i] = testElements[i];
    }
    constexpr unsigned N{2};
    static_assert(N <= SIZE, "N MUST NOT BE BIGGER THAN SIZE");
    buf.eraseNFirst(N);
    testElements.erase(testElements.begin(), std::next(testElements.begin(), N));
    ASSERT_EQ(SIZE - N, buf.size());

    for (auto i = 0U; i < buf.size(); ++i)
    {
        ASSERT_EQ(testElements[i].r, buf[i].r);
        ASSERT_EQ(testElements[i].i, buf[i].i);
    }
}
