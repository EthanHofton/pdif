#include <gtest/gtest.h>
#include <pdif/stream_elem.hpp>

TEST(PDIFStreamElem, TestType) {
    pdif::rstream_elem elem = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    ASSERT_EQ(elem->type(), pdif::stream_type::text);
}

TEST(PDIFStreamElem, TestCreate) {
    pdif::rstream_elem elem = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    ASSERT_EQ(elem->type(), pdif::stream_type::text);
    ASSERT_EQ(elem->as<pdif::text_elem>()->text(), "Hello, World!");
}

TEST(PDIFStreamElem, TestAs) {
    pdif::rstream_elem elem = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    ASSERT_EQ(elem->as<pdif::text_elem>()->text(), "Hello, World!");
}

TEST(PDIFStreamElem, TestAsInvalid) {
    pdif::rstream_elem elem = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    ASSERT_THROW({elem->as<pdif::font_elem>();}, pdif::pdif_invalid_conversion);
}

TEST(PDIFTextElem, TestText) {
    pdif::rtext_elem elem = pdif::stream_elem::create<pdif::text_elem>("Hello, World!")->as<pdif::text_elem>();
    ASSERT_EQ(elem->text(), "Hello, World!");
}

TEST(PDIFTextElem, TestCompareText) {
    pdif::rtext_elem elem1 = pdif::stream_elem::create<pdif::text_elem>("Hello, World!")->as<pdif::text_elem>();
    pdif::rtext_elem elem2 = pdif::stream_elem::create<pdif::text_elem>("Hello, World!")->as<pdif::text_elem>();
    
    ASSERT_TRUE(elem1->compare(elem2));
}

TEST(PDIFTextElem, TestToString) {
    pdif::rtext_elem elem = pdif::stream_elem::create<pdif::text_elem>("Hello, World!")->as<pdif::text_elem>();
    ASSERT_EQ(elem->to_string(), "Hello, World!");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}