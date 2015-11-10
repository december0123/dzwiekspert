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
        file << "tuning=E2,A2,D3,G3,B3,E4" << std::endl;
        p = ConfigParser{PATH};
    }
    void TearDown()
    {
        std::remove(PATH.c_str());
        std::remove("wrong_path");
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

TEST_F(ConfigParserFixture, shouldRestoreDefault)
{
    ASSERT_EQ("right", p.lookup("hand"));
    p.write("hand", "left");
    ASSERT_EQ("left", p.lookup("hand"));
    p.setDefault("hand");
    ASSERT_EQ("right", p.lookup("hand"));
}

TEST_F(ConfigParserFixture, shouldReloadFile)
{
    ASSERT_EQ("right", p.lookup("hand"));
    std::remove(PATH.c_str());
    std::ofstream file{PATH};
    file << "hand=left" << std::endl;
    file << "tuning=E2,A2,D3,G3,B3,E4" << std::endl;
    p.reload();
    ASSERT_EQ("left", p.lookup("hand"));
}

TEST(ConfigParser, shouldCreateConfigIfNotFound)
{
    std::string wrong{"wrong_path"};
    ASSERT_FALSE(std::ifstream(wrong, std::ios::in).good());
    ConfigParser p{wrong};
    ASSERT_TRUE(std::ifstream(wrong, std::ios::in).good());
    ASSERT_EQ("right", p.lookup("hand"));
    ASSERT_EQ("E2,A2,D3,G3,B3,E4", p.lookup("tuning"));
    ASSERT_EQ("440", p.lookup("basic"));
}
