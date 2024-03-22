#include <gtest/gtest.h>
#include <pdif/agl_map.hpp>

TEST(PDIFAglMap, GlyphToUnicodeA) {
    std::string glyph = "A";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "A");
}

TEST(PDIFAglMap, GlyphToUnicodePeriod) {
    std::string glyph = "period";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, ".");
}

TEST(PDIFAglMap, GlyphToUnicodeComma) {
    std::string glyph = "comma";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, ",");
}

TEST(PDIFAglMap, GlyphToUnicodeFi) {
    std::string glyph = "fi";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "fi");
}

TEST(PDIFAglMap, GlyphToUnicodeFfi) {
    std::string glyph = "ffi";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "ffi");
}

TEST(PDIFAglMap, GlyphToUnicodeFfl) {
    std::string glyph = "ffl";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "ffl");
}

TEST(PDIFAglMap, GlyphToUnicodeBackslash) {
    std::string glyph = "backslash";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "\\");
}

TEST(PDIFAglMap, GlyphToUnicodeForAll) {
    std::string glyph = "forall";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "∀");
}

TEST(PDIFAglMap, GlyphToUnicodeRightQuote) {
    std::string glyph = "quoteright";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "’");
}

// test agl-extention
TEST(PDIFAglMap, GlyphToUnicodeNegationslash) {
    std::string glyph = "negationslash";
    std::string unicode = pdif::agl_map::glyph_to_utf8(glyph);
    EXPECT_EQ(unicode, "̸");
}

// test invalid unicode normalization
TEST(PDIFAglMap, GlyphToUnicodeInvalid) {
    EXPECT_THROW(pdif::agl_map::normalizeUTF8("D802"), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}