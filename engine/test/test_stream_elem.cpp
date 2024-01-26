#include <gtest/gtest.h>
#include <pdif/stream_elem.hpp>

TEST(PDIF, TestTextElemText) {
    pdif::text_elem elem("Hello, World!");
    ASSERT_EQ(elem.text(), "Hello, World!");
}

TEST(PDIF, TestTextElemCompareText) {
    pdif::text_elem elem1("Hello, World!");
    pdif::text_elem elem2("Hello, World!");
    
    ASSERT_TRUE(elem1.compare(elem2));
}

TEST(PDIF, TestTextElemCompareBinary) {
    pdif::text_elem elem1("Hello, World!");
    pdif::binary_elem elem2(std::vector<char>{'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'});
    
    ASSERT_FALSE(elem1.compare(elem2));
}

TEST(PDIF, TestBinaryElemBinary) {
    pdif::binary_elem elem(std::vector<char>{'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'});
    ASSERT_EQ(elem.binary(), std::vector<char>({'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'}));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}