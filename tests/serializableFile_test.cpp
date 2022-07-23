#include <gtest/gtest.h>
#include "serializableFile.hpp"

#include <fstream>
#include <memory>

namespace
{
    namespace constants
    {
        constexpr size_t kiloByte = 1024;
        constexpr size_t megaByte = kiloByte * 1024;
        constexpr size_t gigaByte = megaByte * 1024;

        constexpr size_t fileSize = 100 * megaByte;
        constexpr std::string_view fileName = "test_file.txt";
    };

    class serializableFileTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            this->initRandomSeed();
            this->initReferenceVector();

            auto fileContent = this->prepareFileVector();

            this->writeTestFile(fileContent);
        }

        void TearDown() override
        {
            std::remove(constants::fileName.data());
        }

        std::vector<uint8_t> contentReference;

    private:
        void initRandomSeed() const
        {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
        }

        void initReferenceVector()
        {
            contentReference.resize(constants::fileSize);
            std::generate(contentReference.begin(), contentReference.end(), []()
                          { return std::rand(); });
        }

        std::vector<char> const prepareFileVector() const
        {
            std::vector<char> fileContent;
            fileContent.resize(constants::fileSize);
            std::transform(contentReference.begin(), contentReference.end(), fileContent.begin(), [](uint8_t c)
                           { return static_cast<char>(c); });

            return fileContent;
        }

        void writeTestFile(std::vector<char> const &content)
        {
            fileUT.open(constants::fileName.data(), std::ios::binary);
            fileUT.write(content.data(), content.size());
            fileUT.close();
        }

        std::ofstream fileUT;
    };

    TEST_F(serializableFileTest, testing)
    {
        std::unique_ptr<serializable> fileWithSerialization = std::make_unique<serializableFile>(std::string(constants::fileName.data()));

        std::vector<uint8_t> rawBytes = fileWithSerialization->serialize();

        EXPECT_TRUE(std::equal(this->contentReference.begin(), this->contentReference.end(), rawBytes.begin()));
    }
};
