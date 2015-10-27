#include <gtest/gtest.h>
#include "SoundRecognizer.hpp"
#include "Utils.hpp"
#include "Note.hpp"
#include <vector>

class SoundRecognizerFixture : public ::testing::Test
{
protected:
    SoundRecognizer s;
};

TEST_F(SoundRecognizerFixture, shouldRecognizePerfectSingleNotes)
{
    std::vector<Note> notes{{"E2", 82.41_Hz},
                            {"A2", 110.0_Hz}};
    for (const auto& note : notes)
    {
        Note recognizedNote{s.recognizeNote(note.getFreq())};
        ASSERT_EQ(note.getName(), recognizedNote.getName());
    }
}

TEST_F(SoundRecognizerFixture, shouldRecognizeImperfectSingleNotes)
{
    std::vector<Note> notes{{"E2", 82_Hz},
                            {"E2", 84.0_Hz},
                            {"F2", 85_Hz},
                            {"A2", 110.5_Hz},
                            {"A2", 112.12_Hz}};
    for (const auto& note : notes)
    {
        Note recognizedNote{s.recognizeNote(note.getFreq())};
        ASSERT_EQ(note.getName(), recognizedNote.getName());
    }
}

TEST_F(SoundRecognizerFixture, shouldRecognizeImperfectSingleNotesAndCalculateError)
{
    std::vector<Note> notesWithError{{"E2", 82_Hz, -0.005},
                                     {"E2", 84.0_Hz, 0.019},
                                     {"F2", 85_Hz, -0.027},
                                     {"A2", 110.5_Hz, 0.005},
                                     {"A2", 112.12_Hz, 0.019}};
    for (const auto& note : notesWithError)
    {
        Note recognizedNote{s.recognizeNote(note.getFreq())};
        EXPECT_EQ(note.getName(), recognizedNote.getName());
        ASSERT_NEAR(note.getError(), recognizedNote.getError(), 0.0005);
    }
}
