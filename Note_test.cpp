#include "SoundRecognizer.hpp"
#include "Utils.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <string>

class NoteFixture : public ::testing::Test
{
protected:
    int octave{2};
    std::string name{"E"};
    Frequency freq{82.41_Hz};
    double error{0.015};
    Note E2{name + std::to_string(octave), freq, error};
};

TEST_F(NoteFixture, shouldReturnName)
{
    ASSERT_EQ(name, E2.getName());
}

TEST_F(NoteFixture, shouldReturnFullName)
{
    ASSERT_EQ(name + std::to_string(octave), E2.getFullName());
}

TEST_F(NoteFixture, shouldReturnFrequency)
{
    ASSERT_EQ(freq, E2.getFreq());
}

TEST_F(NoteFixture, shouldReturnError)
{
    ASSERT_EQ(freq, E2.getFreq());
}

TEST_F(NoteFixture, shouldBeCopyConstructed)
{
    Note E2_copy{E2};
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
    ASSERT_EQ(error, E2_copy.getError());
}

TEST_F(NoteFixture, shouldBeMoveConstructed)
{
    Note E2_copy{std::move(E2)};
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
    ASSERT_EQ(error, E2_copy.getError());
}

TEST_F(NoteFixture, shouldBeCopyAssigned)
{
    Note E2_copy;
    E2_copy = E2;
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
    ASSERT_EQ(error, E2_copy.getError());
}

TEST_F(NoteFixture, shouldBeMoveAssigned)
{
    Note E2_copy;
    E2_copy = std::move(E2);
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
    ASSERT_EQ(error, E2_copy.getError());
}
