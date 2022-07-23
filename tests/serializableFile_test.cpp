#include <gtest/gtest.h>
#include "serializableFile.hpp"
#include <fstream>

namespace
{
    namespace constants
    {
        constexpr size_t kiloByte = 1024;
        constexpr size_t megaByte = kiloByte  * 1024;
        constexpr size_t gigaByte = megaByte  * 1024;

        constexpr size_t fileSize = 1 * megaByte;
        constexpr std::string_view fileName = "test_file.txt";
    };

    class serializableFileTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            fileUT.open(constants::fileName.data(), std::ios::binary);
            
            std::vector<char> fileContent;
            fileContent.resize(constants::fileSize);
            
            std::generate(fileContent.begin(), fileContent.end(), []() {
                return std::rand();
            });

            fileUT.write(fileContent.data(), fileContent.size());
            fileUT.close();
        }

        void TearDown() override
        {
            std::remove(constants::fileName.data());
        }
    private:
        std::ofstream fileUT;
    };

    TEST_F(serializableFileTest, testing)
    {
        EXPECT_EQ(1, 1);
    }
};
