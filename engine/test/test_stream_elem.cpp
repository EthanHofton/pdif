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

TEST(PDIFTextElem, TestToStringNoCC) {
    pdif::rtext_elem elem = pdif::stream_elem::create<pdif::text_elem>("Hello, World!")->as<pdif::text_elem>();
    ASSERT_EQ(elem->to_string(false), "Hello, World!");
}


TEST(PDIFFontElem, TestFontName) {
    pdif::rfont_elem elem = pdif::stream_elem::create<pdif::font_elem>("Arial", 9)->as<pdif::font_elem>();

    ASSERT_EQ(elem->font_name(), "Arial");
}

TEST(PDIFFontElem, TestFontSize) {
    pdif::rfont_elem elem = pdif::stream_elem::create<pdif::font_elem>("Arial", 9)->as<pdif::font_elem>();

    ASSERT_EQ(elem->font_size(), 9);
}

TEST(PDIFFontElem, TestCompareTrue) {
    pdif::rfont_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9)->as<pdif::font_elem>();
    pdif::rfont_elem elem2 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9)->as<pdif::font_elem>();

    ASSERT_TRUE(elem1->compare(elem2));
}

TEST(PDIFFontElem, TestCompareFalseSize) {
    pdif::rfont_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9)->as<pdif::font_elem>();
    pdif::rfont_elem elem2 = pdif::stream_elem::create<pdif::font_elem>("Arial", 10)->as<pdif::font_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFFontElem, TestCompareFalseName) {
    pdif::rfont_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("CM10", 9)->as<pdif::font_elem>();
    pdif::rfont_elem elem2 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9)->as<pdif::font_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFFontElem, TestCompareFalse) {
    pdif::rfont_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("CM10", 13)->as<pdif::font_elem>();
    pdif::rfont_elem elem2 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9)->as<pdif::font_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFFontElem, TestToString) {
    pdif::rfont_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("CM10", 13)->as<pdif::font_elem>();
    
    std::stringstream ss;
    ss << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    ss << util::CONSOLE_COLOR_CODE::FG_BLUE;
    ss << "[Font set: " << elem1->font_name() << ", " << elem1->font_size() << "pt]";
    ss << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    ss << util::CONSOLE_COLOR_CODE::FG_DEFAULT;

    ASSERT_EQ(elem1->to_string(), ss.str());
}

TEST(PDIFFontElem, TestToStringNoCC) {
    pdif::rfont_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("CM10", 13)->as<pdif::font_elem>();
    
    std::stringstream ss;
    ss << "[Font set: " << elem1->font_name() << ", " << elem1->font_size() << "pt]";

    ASSERT_EQ(elem1->to_string(false), ss.str());

}

TEST(PDIFColorElem, TestRed) {
    pdif::rtext_color_elem elem = pdif::stream_elem::create<pdif::text_color_elem>(0.81 , 0, 0)->as<pdif::text_color_elem>();

    ASSERT_FLOAT_EQ(elem->red(), 0.81);
}

TEST(PDIFColorElem, TestGreen) {
    pdif::rtext_color_elem elem = pdif::stream_elem::create<pdif::text_color_elem>(0, 0.81, 0)->as<pdif::text_color_elem>();

    ASSERT_FLOAT_EQ(elem->green(), 0.81);
}

TEST(PDIFColorElem, TestBlue) {
    pdif::rtext_color_elem elem = pdif::stream_elem::create<pdif::text_color_elem>(0, 0, 0.81)->as<pdif::text_color_elem>();

    ASSERT_FLOAT_EQ(elem->blue(), 0.81);
}

TEST(PDIFTextColorElem, TestCompareTrue) {
    pdif::rtext_color_elem elem1 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();
    pdif::rtext_color_elem elem2 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();

    ASSERT_TRUE(elem1->compare(elem2));
}

TEST(PDIFTextColorElem, TestCompareFalseR) {
    pdif::rtext_color_elem elem1 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();
    pdif::rtext_color_elem elem2 = pdif::stream_elem::create<pdif::text_color_elem>(0.82, 0, 0)->as<pdif::text_color_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFTextColorElem, TestCompareFalseG) {
    pdif::rtext_color_elem elem1 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();
    pdif::rtext_color_elem elem2 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0.01, 0)->as<pdif::text_color_elem>();
}

TEST(PDIFTextColorElem, TestCompareFalseB) {
    pdif::rtext_color_elem elem1 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();
    pdif::rtext_color_elem elem2 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0.01)->as<pdif::text_color_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFTextColorElem, TestToString) {
    pdif::rtext_color_elem elem = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();

    std::stringstream ss;
    ss << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    ss << util::CONSOLE_COLOR_CODE::FG_BLUE;
    ss << "[Text color set: " << elem->red() << "r, " << elem->green() << "g, " << elem->blue() << "b]";
    ss << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    ss << util::CONSOLE_COLOR_CODE::FG_DEFAULT;

    ASSERT_EQ(elem->to_string(), ss.str());
}

TEST(PDIFTextColorElem, TestToStringNoCC) {
    pdif::rtext_color_elem elem = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();

    std::stringstream ss;
    ss << "[Text color set: " << elem->red() << "r, " << elem->green() << "g, " << elem->blue() << "b]";

    ASSERT_EQ(elem->to_string(false), ss.str());
}

TEST(PDIFStrokeColorElem, TestCompareTrue) {
    pdif::rstroke_color_elem elem1 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0)->as<pdif::stroke_color_elem>();
    pdif::rstroke_color_elem elem2 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0)->as<pdif::stroke_color_elem>();

    ASSERT_TRUE(elem1->compare(elem2));
}

TEST(PDIFStrokeColorElem, TestCompareFalseR) {
    pdif::rstroke_color_elem elem1 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0)->as<pdif::stroke_color_elem>();
    pdif::rstroke_color_elem elem2 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.82, 0, 0)->as<pdif::stroke_color_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStrokeColorElem, TestCompareFalseG) {
    pdif::rstroke_color_elem elem1 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0)->as<pdif::stroke_color_elem>();
    pdif::rstroke_color_elem elem2 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0.01, 0)->as<pdif::stroke_color_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStrokeColorElem, TestCompareFalseB) {
    pdif::rstroke_color_elem elem1 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0)->as<pdif::stroke_color_elem>();
    pdif::rstroke_color_elem elem2 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0.01)->as<pdif::stroke_color_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStrokeColorElem, TestToString) {
    pdif::rstroke_color_elem elem = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0)->as<pdif::stroke_color_elem>();

    std::stringstream ss;
    ss << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    ss << util::CONSOLE_COLOR_CODE::FG_BLUE;
    ss << "[Stroke color set: " << elem->red() << "r, " << elem->green() << "g, " << elem->blue() << "b]";
    ss << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    ss << util::CONSOLE_COLOR_CODE::FG_DEFAULT;

    ASSERT_EQ(elem->to_string(), ss.str());
}

TEST(PDIFStrokeColorElem, TestToStringNoCC) {
    pdif::rtext_color_elem elem = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0)->as<pdif::text_color_elem>();

    std::stringstream ss;
    ss << "[Text color set: " << elem->red() << "r, " << elem->green() << "g, " << elem->blue() << "b]";

    ASSERT_EQ(elem->to_string(false), ss.str());
}

TEST(PDIFXObjectImgElem, TestImageHash) {
    pdif::rxobject_img_elem elem = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();

    ASSERT_EQ(elem->image_hash(), "faf89233299323faf3f3");
}

TEST(PDIFXObjectImgElem, TestWidth) {
    pdif::rxobject_img_elem elem = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();

    ASSERT_EQ(elem->width(), 300);
}

TEST(PDIFXObjectImgElem, TestHeight) {
    pdif::rxobject_img_elem elem = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();

    ASSERT_EQ(elem->height(), 300);
}

TEST(PDIFXObjectImgElem, TestCompareTrue) {
    pdif::rxobject_img_elem elem1 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();
    pdif::rxobject_img_elem elem2 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();

    ASSERT_TRUE(elem1->compare(elem2));
}
TEST(PDIFXObjectImgElem, TestCompareFalseHash) {
    pdif::rxobject_img_elem elem1 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();
    pdif::rxobject_img_elem elem2 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f4", 300, 300)->as<pdif::xobject_img_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}
TEST(PDIFXObjectImgElem, TestCompareFalseWidth) {
    pdif::rxobject_img_elem elem1 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();
    pdif::rxobject_img_elem elem2 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 301, 300)->as<pdif::xobject_img_elem>();

    ASSERT_FALSE(elem1->compare(elem2));

}
TEST(PDIFXObjectImgElem, TestCompareFalseHeight) {
    pdif::rxobject_img_elem elem1 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();
    pdif::rxobject_img_elem elem2 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 301)->as<pdif::xobject_img_elem>();

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFXObjectImgElem, TestToString) {
    pdif::rxobject_img_elem elem = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();

    std::stringstream ss;
    ss << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    ss << util::CONSOLE_COLOR_CODE::FG_LIGHT_MAGENTA;
    ss << "[Image: (sha1)" << elem->image_hash() << ", " << elem->width() << "x" << elem->height() << "px]";
    ss << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    ss << util::CONSOLE_COLOR_CODE::FG_DEFAULT;

    ASSERT_EQ(elem->to_string(), ss.str());
}

TEST(PDIFXObjectImgElem, TestToStringNoCC) {
    pdif::rxobject_img_elem elem = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300)->as<pdif::xobject_img_elem>();

    std::stringstream ss;
    ss << "[Image: (sha1)" << elem->image_hash() << ", " << elem->width() << "x" << elem->height() << "px]";

    ASSERT_EQ(elem->to_string(false), ss.str());
}

TEST(PDIFStreamElem, TestCompareDifferentTypeTextFont) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeTextTextColor) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeTextStrokeColor) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeTextXObjectImg) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::text_elem>("Hello, World!");
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeFontTextColor) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9);
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeFontStrokeColor) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9);
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeFontXObjectImg) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::font_elem>("Arial", 9);
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeTextColorStrokeColor) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0);
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::stroke_color_elem>(0.81, 0, 0);

    ASSERT_FALSE(elem1->compare(elem2));
}

TEST(PDIFStreamElem, TestCompareDifferentTypeTextColorXObjectImg) {
    pdif::rstream_elem elem1 = pdif::stream_elem::create<pdif::text_color_elem>(0.81, 0, 0);
    pdif::rstream_elem elem2 = pdif::stream_elem::create<pdif::xobject_img_elem>("faf89233299323faf3f3", 300, 300);

    ASSERT_FALSE(elem1->compare(elem2));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}