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

TEST_F(ConfigParserFixture, shouldThrowOnMalformed)
{
    ASSERT_NO_THROW(p.lookup("hand"));
    ASSERT_THROW(p.lookup("fake"), std::logic_error);
    std::string MALFORMED_PATH{"test_malformed_file"};
    std::ofstream malformed{MALFORMED_PATH};
    malformed << "MALFORMED" << std::endl;
//    ASSERT_THROW(ConfigParser(MALFORMED_PATH), std::logic_error);

    std::remove(MALFORMED_PATH.c_str());
}
