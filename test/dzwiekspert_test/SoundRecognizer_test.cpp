#include <gtest/gtest.h>
#include "../../SoundRecognizer.hpp"
#include "../../Utils.hpp"
#include <string>

TEST(SoundRecognizer, shouldRecognizeSound_E2)
{
    constexpr Frequency E2 = 82_Hz;
    const std::string ideal{"E2"};
    SoundRecognizer s;
    ASSERT_EQ(ideal, s.recognizeNote(E2));
}
