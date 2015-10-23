#include <gtest/gtest.h>
#include "SoundRecognizer.hpp"
#include "Utils.hpp"
#include <vector>
#include <string>

class NoteFixture : public ::testing::Test
{
protected:
    std::string name{"E2"};
    Frequency freq{82.41_Hz};
    Note E2{name, freq};
};

TEST_F(NoteFixture, shouldReturnName)
{
    ASSERT_EQ(name, E2.getName());
}

TEST_F(NoteFixture, shouldReturnFrequency)
{
    ASSERT_EQ(freq, E2.getFreq());
}

TEST_F(NoteFixture, shouldBeCopyConstructed)
{
    Note E2_copy{E2};
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
}

TEST_F(NoteFixture, shouldBeMoveConstructed)
{
    Note E2_copy{std::move(E2)};
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
}

TEST_F(NoteFixture, shouldBeCopyAssigned)
{
    Note E2_copy;
    E2_copy = E2;
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
}

TEST_F(NoteFixture, shouldBeMoveAssigned)
{
    Note E2_copy;
    E2_copy = std::move(E2);
    ASSERT_EQ(freq, E2_copy.getFreq());
    ASSERT_EQ(name, E2_copy.getName());
}
