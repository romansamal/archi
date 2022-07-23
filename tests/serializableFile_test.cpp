#include <gtest/gtest.h>
#include "serializableFile.hpp"

namespace
{
    class serializableFileTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
        }

        void TearDown() override
        {
        }
    };

    TEST_F(serializableFileTest, testing)
    {
        EXPECT_EQ(1, 1);
    }
};
