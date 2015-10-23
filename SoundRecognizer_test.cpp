#include <gtest/gtest.h>
#include "SoundRecognizer.hpp"
#include "Utils.hpp"
#include "Note.hpp"
#include <iostream>
#include <vector>
#include <string>

TEST(SoundRecognizer, shouldRecognizePerfectSingleNotes)
{
    SoundRecognizer s;
    std::vector<Note> sounds{{"E2", 82.41_Hz}, {"A2", 110.0_Hz}};
    for (const auto& sound : sounds)
    {
        ASSERT_EQ(sound.getName(), s.recognizeNote(sound.getFreq()));
    }
}

TEST(SoundRecognizer, shouldRecognizeImperfectSingleNotes)
{
    SoundRecognizer s;
    std::vector<Note> sounds{{"E2", 82_Hz}, {"E2", 84.0_Hz}, {"A2", 110.5_Hz}, {"A2", 112.12_Hz}};
    for (const auto& sound : sounds)
    {
        ASSERT_EQ(sound.getName(), s.recognizeNote(sound.getFreq()));
    }
}
