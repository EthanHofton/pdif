#include <gtest/gtest.h>
#include <pdif/content_extractor.hpp>
#include <pdif/stream_elem.hpp>
#include <qpdf/QPDF.hh>

TEST(PDIFContentExtractor, TestExtractMeta) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/metadata_initial.pdf");

    pdif::stream_meta meta = pdif::extract_meta(pdf);

    ASSERT_EQ(meta.get_metadata("Keywords"), "Test Keywords");
    ASSERT_EQ(meta.get_metadata("Author"), "Test Author");
    ASSERT_EQ(meta.get_metadata("Subject"), "Test Subject");
    ASSERT_EQ(meta.get_metadata("Title"), "Test Title");
}

TEST(PDIFContentExtractor, TextGWord) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/content_initial.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::word, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 4);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "Hello");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), "world");

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), "1");
}

TEST(PDIFContentExtractor, TextGLetter) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/content_initial.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::letter, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 12);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    std::string expected = "Helloworld1";
    for (int i = 1; i < 12; i++) {
        ASSERT_EQ(s[i]->type(), pdif::stream_type::text);
        std::string t = std::string(1, expected[i-1]);
        ASSERT_EQ(s[i]->as<pdif::text_elem>()->text(), t);
    }
}

TEST(PDIFContentExtractor, TextGSentence) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/content_initial.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::sentence, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 2);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "Hello world 1");
}

TEST(PDIFContentExtractor, TestGSentenceMultiple) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/multi_sentence.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::sentence, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 7);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "Hello World.");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), "This is the first sentence.");

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), "This is the second.");

    ASSERT_EQ(s[4]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[4]->as<pdif::text_elem>()->text(), "This is the last sentence.");

    ASSERT_EQ(s[5]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[5]->as<pdif::text_elem>()->text(), "There is no space between this and the last.");

    ASSERT_EQ(s[6]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[6]->as<pdif::text_elem>()->text(), "1");
}

TEST(PDIFContentExtractor, Ligature) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/ligature.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::sentence, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 4);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "This is an example of a ligature.");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), "fi.");

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), "1");
}

TEST(PDIFContentExtractor, LigatureInWord) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/ligature_in_word.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::sentence, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 4);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "This is an example of a ligature in a word.");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), "file.");

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), "1");
}

TEST(PDIFContentExtractor, Image) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/image_initial.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::sentence, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 6);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "The universe is immense and it seems to be homogeneous, on a large scale, everywhere we look.");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::xobject_image);
    ASSERT_EQ(s[2]->as<pdif::xobject_img_elem>()->image_hash(), "bc35317497c60e50332101c357ad9f2139dd728b");
    ASSERT_EQ(s[2]->as<pdif::xobject_img_elem>()->width(), 425);
    ASSERT_EQ(s[2]->as<pdif::xobject_img_elem>()->height(), 307);

    ASSERT_EQ(s[3]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[3]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[3]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[4]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[4]->as<pdif::text_elem>()->text(), "There's a picture of a galaxy above.");

    ASSERT_EQ(s[5]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[5]->as<pdif::text_elem>()->text(), "1");
}

TEST(PDIFContentExtractor, Fonts) {
    std::shared_ptr<QPDF> pdf = QPDF::create();
    pdf->processFile("test_pdfs/multi_font.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::sentence, pdif::scope::page);

    ASSERT_EQ(streams.size(), 1);

    pdif::stream s = streams[0];

    ASSERT_EQ(s.size(), 11);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[0]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "This is an example of a simple font change:");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[2]->as<pdif::font_elem>()->font_name(), "CMBX10");
    ASSERT_EQ(s[2]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), "bold");

    ASSERT_EQ(s[4]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[4]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[4]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[5]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[5]->as<pdif::text_elem>()->text(), "and");

    ASSERT_EQ(s[6]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[6]->as<pdif::font_elem>()->font_name(), "CMTI10");
    ASSERT_EQ(s[6]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[7]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[7]->as<pdif::text_elem>()->text(), "italic");

    ASSERT_EQ(s[8]->type(), pdif::stream_type::font_set);
    ASSERT_EQ(s[8]->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(s[8]->as<pdif::font_elem>()->font_size(), 9);

    ASSERT_EQ(s[9]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[9]->as<pdif::text_elem>()->text(), ".");

    ASSERT_EQ(s[10]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[10]->as<pdif::text_elem>()->text(), "1");
}

TEST(PDIFContentExtractor, Color) {}
TEST(PDIFContentExtractor, Backslash) {}

TEST(PDIFContentExtractor, Pageno2) {}
TEST(PDIFContentExtractor, Pageno3) {}
TEST(PDIFContentExtractor, PagenoAll) {}
TEST(PDIFContentExtractor, PagenoInvalid) {}

TEST(PDIFContentExtractor, MultiPageScopeDocument) {}
TEST(PDIFContentExtractor, MultiPageScopePage) {}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}