#include <gtest/gtest.h>
#include "SoundRecognizer.hpp"
#include "Utils.hpp"
#include <vector>
#include <string>

TEST(Note, shouldReturnName)
{
    const std::string name{"E2"};
    const Frequency freq{82_Hz};
    Note E2{name, freq};
    ASSERT_EQ(name, E2.getName());
}

TEST(Note, shouldReturnFrequency)
{
    const std::string name{"E2"};
    const Frequency freq{82_Hz};
    Note E2{name, freq};
    ASSERT_EQ(freq, E2.getFreq());
}
