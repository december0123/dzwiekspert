#include <gtest/gtest.h>
#include "SoundRecognizer.hpp"
#include "Utils.hpp"
#include "Note.hpp"
#include <iostream>
#include <vector>
#include <string>

class SoundRecognizerFixture : public ::testing::Test
{
protected:
    SoundRecognizer s;
};

TEST_F(SoundRecognizerFixture, shouldRecognizePerfectSingleNotes)
{
    std::vector<Note> notes{{"E2", 82.41_Hz}, {"A2", 110.0_Hz}};
    for (const auto& note : notes)
    {
        ASSERT_EQ(note.getName(), s.recognizeNote(note.getFreq()));
    }
}

TEST_F(SoundRecognizerFixture, shouldRecognizeImperfectSingleNotes)
{
    std::vector<Note> notes{{"E2", 82_Hz}, {"E2", 84.0_Hz}, {"F2", 85_Hz}, {"A2", 110.5_Hz}, {"A2", 112.12_Hz}};
    for (const auto& note : notes)
    {
        ASSERT_EQ(note.getName(), s.recognizeNote(note.getFreq()));
    }
}
