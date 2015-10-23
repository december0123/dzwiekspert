#include <gtest/gtest.h>
#include "SoundRecognizer.hpp"
#include "Utils.hpp"
#include "Note.hpp"
#include <iostream>
#include <vector>
#include <string>

TEST(SoundRecognizer, shouldRecognizePerfectSingleNotesFromE2ToA3)
{
    std::cout << "BEGIN shouldRecognizePerfectSingleNotesFromE2ToA3" << std::endl;
    SoundRecognizer s;
    std::vector<Note> sounds{{"E2", 82.41_Hz}, {"A2", 110.0_Hz}};
    for (const auto& sound : sounds)
    {
        std::cout << "TEST: " << sound.getName() << std::endl;
        ASSERT_EQ(sound.getName(), s.recognizeNote(sound.getFreq()));
    }
}

TEST(SoundRecognizer, shouldRecognizeImperfectSingleNotesFromE2ToA3)
{
    SoundRecognizer s;
    std::vector<Note> sounds{{"E2", 84.0_Hz}, {"A2", 110.5_Hz}};
    for (const auto& sound : sounds)
    {
        ASSERT_EQ(sound.getName(), s.recognizeNote(sound.getFreq()));
    }
}
