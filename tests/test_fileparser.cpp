#include "gtest/gtest.h"
#include "../fileparser.h"
#include <fstream>

TEST(FileParserTest, CorrectlyIdentifiesCppFiles) {
    FileParser parser;
    parser.parseDirectory("../tests", {".cpp", ".h"});
    const auto& files = parser.getDirectories();
    ASSERT_GT(files.size(), 0);
    for (const auto& file : files) {
        ASSERT_TRUE(file.extension() == ".cpp" || file.extension() == ".h");
    }
}

TEST(FileParserTest, CorrectlyIdentifiesMarkdownFiles) {
    FileParser parser;
    parser.parseDirectory(".", {".md"});
    const auto& files = parser.getDirectories();
    // Assuming there is at least one markdown file for testing
    if (files.size() > 0) {
        for (const auto& file : files) {
            ASSERT_EQ(file.extension(), ".md");
        }
    }
}

TEST(FileParserTest, CorrectlyIdentifiesTextFiles) {
    FileParser parser;
    parser.parseDirectory(".", {".txt"});
    const auto& files = parser.getDirectories();
    // Assuming there is at least one text file for testing
    if (files.size() > 0) {
        for (const auto& file : files) {
            ASSERT_EQ(file.extension(), ".txt");
        }
    }
}
