#include "Note.hpp"
#include "SoundRecognizer.hpp"
#include "Utils.hpp"

#include <gtest/gtest.h>

#include <vector>

class SoundRecognizerFixture : public ::testing::Test
{
protected:
    const Frequency BASIC_FREQ{65.41_Hz};
    SoundRecognizer s{BASIC_FREQ};
};

TEST_F(SoundRecognizerFixture, shouldRecognizePerfectSingleNotes)
{
    std::vector<Note> notes{{"E2",      82.41_Hz},
                            {"F2",      87.31_Hz},
                            {"A2",      110.0_Hz},
                            {"C5",      523.25_Hz,},
                            {"F#6",     1479.98_Hz}};
    for (const auto& note : notes)
    {
        Note recognizedNote{s.recognizeNote(note.getFreq())};
        ASSERT_EQ(note.getFullName(), recognizedNote.getFullName());
    }
}

TEST_F(SoundRecognizerFixture, shouldRecognizeImperfectSingleNotesAndCalculateError)
{
    std::vector<Note> notesWithError{{"E2", 82_Hz, -0.005},
                                     {"E2", 84.0_Hz, 0.019},
                                     {"F2", 85_Hz, -0.027},
                                     {"A2", 110.5_Hz, 0.004},
                                     {"A2", 112.12_Hz, 0.019},
                                     {"C5", 530_Hz, 0.013}};
    for (const auto& note : notesWithError)
    {
        Note recognizedNote{s.recognizeNote(note.getFreq())};
        EXPECT_EQ(note.getFullName(), recognizedNote.getFullName());
        ASSERT_NEAR(note.getError(), recognizedNote.getError(), 0.0005);
    }
}

//    std::vector<Note> notes_{{"C2",     65.41_Hz},
//                             {"C#2",    69.30_Hz},
//                             {"D2",     73.42_Hz},
//                             {"D#2",    77.78_Hz},
//                             {"E2",     82.41_Hz},
//                             {"F2",     87.31_Hz},
//                             {"F#2",    92.50_Hz},
//                             {"G2",     98.00_Hz},
//                             {"G#2",    103.83_Hz},
//                             {"A2",     110.00_Hz},
//                             {"A#2",    116.54_Hz},
//                             {"B2",     123.47_Hz},

//                             {"C3",     130.81_Hz},
//                             {"C#3",    138.59_Hz},
//                             {"D3",     146.83_Hz},
//                             {"D#3",    155.56_Hz},
//                             {"E3",     164.81_Hz},
//                             {"F3",     174.61_Hz},
//                             {"F#3",    185.00_Hz},
//                             {"G3",     196.00_Hz},
//                             {"G#3",    207.65_Hz},
//                             {"A3",     220.00_Hz},
//                             {"A#3",    233.08_Hz},
//                             {"B3",     246.94_Hz},

//                             {"C4",     261.63_Hz},
//                             {"C#4",    277.18_Hz},
//                             {"D4",     293.66_Hz},
//                             {"D#4",    311.13_Hz},
//                             {"E4",     329.63_Hz},
//                             {"F4",     349.23_Hz},
//                             {"F#4",    369.99_Hz},
//                             {"G4",     392.00_Hz},
//                             {"G#4",    415.30_Hz},
//                             {"A4",     440.00_Hz},
//                             {"A#4",    466.16_Hz},
//                             {"B4",     493.88_Hz},

//                             {"C5",     523.25_Hz},
//                             {"C#5",    554.37_Hz},
//                             {"D5",     587.33_Hz},
//                             {"D#5",    622.25_Hz},
//                             {"E5",     659.25_Hz},
//                             {"F5",     698.46_Hz},
//                             {"F#5",    739.99_Hz},
//                             {"G5",     783.99_Hz},
//                             {"G#5",    830.61_Hz},
//                             {"A5",     880.00_Hz},
//                             {"A#5",    932.33_Hz},
//                             {"B5",     987.77_Hz},

//                             {"C6",     1046.50_Hz},
//                             {"C#6",    1108.73_Hz},
//                             {"D6",     1174.66_Hz},
//                             {"D#6",    1244.51_Hz},
//                             {"E6",     1318.51_Hz},
//                             {"F6",     1396.91_Hz},
//                             {"F#6",    1479.98_Hz},
//                             {"G6",     1567.98_Hz},
//                             {"G#6",    1661.22_Hz},
//                             {"A6",     1760.00_Hz},
//                             {"A#6",    1864.66_Hz},
//                             {"B6",     1975.53_Hz},
//                             };
