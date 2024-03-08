#include <gtest/gtest.h>
#include <pdif/lcs_stream_differ.hpp>
#include <pdif/pdf.hpp>

TEST(PDIFPDF, Constructor) {
    ASSERT_NO_THROW({pdif::PDF pdf("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page);});
}

TEST(PDIFPDF, GetGranularity) {
    pdif::PDF pdf("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page);
    ASSERT_EQ(pdf.get_granularity(), pdif::granularity::word);
}

TEST(PDIFPDF, GetScope) {
    pdif::PDF pdf("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page);
    ASSERT_EQ(pdf.get_scope(), pdif::scope::page);
}

TEST(PDIFPDF, WriteConsoleColors) {
    pdif::PDF pdf("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page, true);
    ASSERT_TRUE(pdf.write_console_colors());
}

TEST(PDIFPDF, WriteConsoleColorsSet) {
    pdif::PDF pdf("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page, false);
    ASSERT_FALSE(pdf.write_console_colors());

    pdf.write_console_colors(true);

    ASSERT_TRUE(pdf.write_console_colors());
}

TEST(PDIFPDF, DumpMetaNoConsoleColors) {
    pdif::PDF pdf("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page, false);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_meta(pdf_ss));

    std::stringstream ss;
    ss << "Meta:" << std::endl;
    ss << std::endl;

    for (auto& [key, value] : pdf.get_meta().get_metadata()) {
        ss << key << ": " << value << std::endl;
    }

    ss << std::endl;
    ss << "== Meta Finished ==" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpMetaConsoleColors) {
    pdif::PDF pdf("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page, true);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_meta(pdf_ss));

    std::stringstream ss;
    ss << "\033[1mMeta:\033[0m" << std::endl;
    ss << std::endl;

    for (auto& [key, value] : pdf.get_meta().get_metadata()) {
        ss << key << ": " << value << std::endl;
    }

    ss << std::endl;
    ss << "\033[1m== Meta Finished ==\033[0m" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpContentNoConsoleColorsPageno0) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, false);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss, 0));

    std::stringstream ss;
    pdf.dump_meta(ss);

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

// all
TEST(PDIFPDF, DumpContentNoConsoleColorsPageNoNegative) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, false);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss, -1));

    std::stringstream ss;

    ss << "Page 1:" << std::endl;
    ss << std::endl;

    auto page = pdf.get_streams()[0];

    for (size_t i = 0; i < page.size(); i++) {
        ss << page[i]->to_string() << " ";
    }

    ss << std::endl;
    ss << "== Page 1 Finished ==" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpContentNoConsoleColorsPageSingle) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, false);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss, 1));

    std::stringstream ss;

    ss << "Page 1:" << std::endl;
    ss << std::endl;

    auto page = pdf.get_streams()[0];

    for (size_t i = 0; i < page.size(); i++) {
        ss << page[i]->to_string() << " ";
    }

    ss << std::endl;
    ss << "== Page 1 Finished ==" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpContentPageNoInvalid) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, false);

    std::stringstream pdf_ss;

    ASSERT_THROW(pdf.dump_content(pdf_ss, 2), std::runtime_error);
}

TEST(PDIFPDF, DumpContentConsoleColorsPageNo0) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, true);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss, 0));

    std::stringstream ss;
    pdf.dump_meta(ss);

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpContentConsoleColorsPageSingle) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, true);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss, 1));

    std::stringstream ss;

    ss << "\033[1mPage 1:\033[0m" << std::endl;
    ss << std::endl;

    auto page = pdf.get_streams()[0];

    for (size_t i = 0; i < page.size(); i++) {
        ss << page[i]->to_string() << " ";
    }

    ss << std::endl;
    ss << "\033[1m== Page 1 Finished ==\033[0m" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpContentConsoleColorsPageNoNegative) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, true);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss, -1));

    std::stringstream ss;

    ss << "\033[1mPage 1:\033[0m" << std::endl;
    ss << std::endl;

    auto page = pdf.get_streams()[0];

    for (size_t i = 0; i < page.size(); i++) {
        ss << page[i]->to_string() << " ";
    }

    ss << std::endl;
    ss << "\033[1m== Page 1 Finished ==\033[0m" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

/**
 * @brief From Here on we will test the compare function
 * 
 */

// Meta Changes
TEST(PDIFPDFCompare, Metadata) {
    pdif::PDF pdf1("test_pdfs/metadata_initial.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/metadata_changed.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.meta_edit_op_size(), 5);
    
    auto op = d.get_meta_edit_op(0);
    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_UPDATE);
    ASSERT_EQ(op.get_meta_key(), "CreationDate");
    ASSERT_EQ(op.get_meta_val(), "D:20240301121344Z");

    op = d.get_meta_edit_op(1);
    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_UPDATE);
    ASSERT_EQ(op.get_meta_key(), "ModDate");
    ASSERT_EQ(op.get_meta_val(), "D:20240301121344Z");

    op = d.get_meta_edit_op(2);
    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_ADD);
    ASSERT_EQ(op.get_meta_key(), "TestField");
    ASSERT_EQ(op.get_meta_val(), "Added");

    op = d.get_meta_edit_op(3);
    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_UPDATE);
    ASSERT_EQ(op.get_meta_key(), "Title");
    ASSERT_EQ(op.get_meta_val(), "Title Changed");

    op = d.get_meta_edit_op(4);
    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_DELETE);
    ASSERT_EQ(op.get_meta_key(), "Author");
}

// Text Changes
TEST(PDIFPDFCompare, BasicTextWordGranularity) {
    pdif::PDF pdf1("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/content_final.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 5);
    
    auto op = d.get_edit_op(0);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "my");

    op = d.get_edit_op(3);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(4);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, BasicTextLetterGranularity) {
    pdif::PDF pdf1("test_pdfs/content_initial.pdf", pdif::granularity::letter, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/content_final.pdf", pdif::granularity::letter, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 14);
    
    for (int i = 0; i < 5; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(6);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "m");

    op = d.get_edit_op(7);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "y");

    for (int i = 8; i < 14; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

TEST(PDIFPDFCompare, BasicTextSentenceGranularity) {
    pdif::PDF pdf1("test_pdfs/content_initial.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/content_final.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 3);
    
    auto op = d.get_edit_op(0);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Hello my world1");

    op = d.get_edit_op(2);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);
}

TEST(PDIFPDFCompare, ComplexTextLetterGranularityScopePage) {}

TEST(PDIFPDFCompare, ComplexTextLetterGranularityScopeDocument) {}

TEST(PDIFPDFCompare, ComplexTextWordGranularityScopePage) {}
TEST(PDIFPDFCompare, ComplexTextWordGranularityScopeDocument) {}

TEST(PDIFPDFCompare, ComplexTextSentenceGranularityScopePage) {}
TEST(PDIFPDFCompare, ComplexTextSentenceGranularityScopeDocument) {}

// Font Changes
TEST(PDIFPDFCompare, FontChangeNormalToBold) {}
TEST(PDIFPDFCompare, FontChangeNormalToRed) {}
TEST(PDIFPDFCompare, FontChangeRedToBold) {}
TEST(PDIFPDFCompare, FontChangeRedToNormal) {}
TEST(PDIFPDFCompare, FontChangeBoldToNormal) {}
TEST(PDIFPDFCompare, FontChangeBoldToRed) {}
TEST(PDIFPDFCompare, FontChangeRedToItalicRed) {}
TEST(PDIFPDFCompare, FontChangeRedItalicToRed) {}
TEST(PDIFPDFCompare, FontChangeRedItalicToBold) {}
TEST(PDIFPDFCompare, FontChangeRedItalicToNormal) {}
TEST(PDIFPDFCompare, FontChangeNormalToRedItalic) {}
TEST(PDIFPDFCompare, FontChangeBoldToRedItalic) {}

// Image Changes
TEST(PDIFPDFCompare, ImageAddedSpace) {}
TEST(PDIFPDFCompare, ImageAddedDonut) {}
TEST(PDIFPDFCompare, ImageRemovedSpace) {}
TEST(PDIFPDFCompare, ImageRemovedDonut) {}
TEST(PDIFPDFCompare, ImageChangedSpaceToDonut) {}
TEST(PDIFPDFCompare, ImageChangedDonutToSpace) {}
TEST(PDIFPDFCompare, ImageTextAdded) {}
TEST(PDIFPDFCompare, ImageTextRemoved) {}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}