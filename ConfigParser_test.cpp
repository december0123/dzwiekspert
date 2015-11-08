#include "ConfigParser.hpp"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <vector>

class ConfigParserFixture : public ::testing::Test
{
protected:
    std::string PATH{"test_config"};
    std::ofstream file{PATH};
    ConfigParser p{PATH};
    void SetUp()
    {
        file << "hand=right" << std::endl;
        file << "tuning=E,A,D,G,B,E" << std::endl;
        p = ConfigParser{PATH};
    }
    void TearDown()
    {
        std::remove(PATH.c_str());
    }
};

TEST_F(ConfigParserFixture, shouldReadConfig)
{
    ASSERT_EQ("right", p.lookup("hand"));
}

TEST_F(ConfigParserFixture, shouldWriteConfig)
{
    p.write("hand", "left");
    ASSERT_EQ("left", p.lookup("hand"));
}

TEST_F(ConfigParserFixture, shouldSaveConfig)
{
    ASSERT_EQ("right", p.lookup("hand"));
    p.write("hand", "left");
    ASSERT_EQ("left", p.lookup("hand"));
    p.save();
    ConfigParser p2{PATH};
    ASSERT_EQ("left", p2.lookup("hand"));
}

TEST_F(ConfigParserFixture, shouldThrowOnNotFound)
{
    ASSERT_NO_THROW(p.lookup("hand"));
    ASSERT_THROW(p.lookup("fake"), std::logic_error);
}

TEST(ConfigParser, shouldCreateConfigIfNotFound)
{
    std::string wrong{"wrong_path"};
    ASSERT_FALSE(std::ifstream(wrong, std::ios::in).good());
    ConfigParser p{wrong};
    ASSERT_TRUE(std::ifstream(wrong, std::ios::in).good());
    ASSERT_EQ("right", p.lookup("hand"));
    ASSERT_EQ("EADGBE", p.lookup("tuning"));
    ASSERT_EQ("65.41", p.lookup("basic"));
    std::remove(wrong.c_str());
}
