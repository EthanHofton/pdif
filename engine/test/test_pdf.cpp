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

// all
TEST(PDIFPDF, DumpContentNoConsoleColorsPageNoNegative) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, false, -1);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss));

    std::stringstream ss;

    ss << "Page 1:" << std::endl;
    ss << std::endl;

    auto page = pdf.get_streams()[0];

    for (size_t i = 0; i < page.size(); i++) {
        ss << page[i]->to_string(false) << " ";
    }

    ss << std::endl;
    ss << "== Page 1 Finished ==" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpContentNoConsoleColorsPageSingle) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, false, 0);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss));

    std::stringstream ss;

    ss << "Page 1:" << std::endl;
    ss << std::endl;

    auto page = pdf.get_streams()[0];

    for (size_t i = 0; i < page.size(); i++) {
        ss << page[i]->to_string(false) << " ";
    }

    ss << std::endl;
    ss << "== Page 1 Finished ==" << std::endl;

    ASSERT_EQ(pdf_ss.str(), ss.str());
}

TEST(PDIFPDF, DumpContentPageNoInvalid) {
    ASSERT_THROW({pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, false, 1);}, std::runtime_error);
}

TEST(PDIFPDF, DumpContentConsoleColorsPageSingle) {
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, true, 0);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss));

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
    pdif::PDF pdf("test_pdfs/content_initial.pdf", pdif::granularity::word, pdif::scope::page, true, -1);

    std::stringstream pdf_ss;

    ASSERT_NO_THROW(pdf.dump_content(pdf_ss));

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
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Hello my world 1");

    op = d.get_edit_op(2);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);
}

// Font Changes
TEST(PDIFPDFCompare, FontChangeNormalToBold) {
    pdif::PDF pdf1("test_pdfs/font_change_inital.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_bold.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 22);

    for (int i = 0; i < 6; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(6);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(7);
    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "This example shows how to use the");
    
    op = d.get_edit_op(8);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMBX10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(9);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "xcolor");

    op = d.get_edit_op(10);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(11);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "package to change the color of L");

    op = d.get_edit_op(12);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    for (int i = 13; i < 22; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

TEST(PDIFPDFCompare, FontChangeNormalToRed) {
    pdif::PDF pdf1("test_pdfs/font_change_inital.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_red.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 39);

    for (int i = 0; i < 14; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
    
    auto op = d.get_edit_op(14);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->red(), 1);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->blue(), 0);

    op = d.get_edit_op(15);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::stroke_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->red(), 1);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->blue(), 0);

    op = d.get_edit_op(16);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(17);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->red(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->blue(), 0);

    op = d.get_edit_op(18);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::stroke_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->red(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->blue(), 0);

    for (int i = 19; i < 39; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

TEST(PDIFPDFCompare, FontChangeRedToBold) {
    pdif::PDF pdf1("test_pdfs/font_change_red.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_bold.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 41);

    for (int i = 0; i < 14; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(14);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMBX10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(15);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(16);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(17);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(18);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(19);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(20);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    for (int i = 21; i < 41; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

TEST(PDIFPDFCompare, FontChangeRedToNormal) {
    pdif::PDF pdf1("test_pdfs/font_change_red.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_inital.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 39);

    for (int i = 0; i < 14; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
    
    auto op = d.get_edit_op(14);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(15);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(16);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(17);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(18);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    for (int i = 19; i < 39; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

TEST(PDIFPDFCompare, FontChangeBoldToNormal) {
    pdif::PDF pdf1("test_pdfs/font_change_bold.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_inital.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 37);

    for (int i = 0; i < 14; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
    
    auto op = d.get_edit_op(14);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(15);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(16);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    for (int i = 17; i < 37; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

TEST(PDIFPDFCompare, FontChangeBoldToRed) {
    pdif::PDF pdf1("test_pdfs/font_change_bold.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_red.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 41);

    for (int i = 0; i < 14; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(14);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->red(), 1);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->blue(), 0);

    op = d.get_edit_op(15);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::stroke_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->red(), 1);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->blue(), 0);

    op = d.get_edit_op(16);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(17);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(18);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->red(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::text_color_elem>()->blue(), 0);

    op = d.get_edit_op(19);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::stroke_color_set);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->red(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->green(), 0);
    ASSERT_EQ(op.get_arg()->as<pdif::stroke_color_elem>()->blue(), 0);

    op = d.get_edit_op(20);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    for (int i = 21; i < 41; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

TEST(PDIFPDFCompare, FontChangeRedToItalicRed) {
    pdif::PDF pdf1("test_pdfs/font_change_red.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_red_it.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 41);

    for (int i = 0; i < 16; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(16);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMTI10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(17);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(18);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(19);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(20);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    for (int i = 21; i < 41; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}
TEST(PDIFPDFCompare, FontChangeRedItalicToRed) {
    pdif::PDF pdf1("test_pdfs/font_change_red_it.pdf", pdif::granularity::word, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/font_change_red.pdf", pdif::granularity::word, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 41);

    for (int i = 0; i < 16; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(16);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(17);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(18);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(19);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(20);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    for (int i = 21; i < 41; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }
}

// Image Changes
TEST(PDIFPDFCompare, ImageAddedSpace) {
    pdif::PDF pdf1("test_pdfs/image_no_image.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_initial.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 6);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::xobject_image);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->image_hash(), "bc35317497c60e50332101c357ad9f2139dd728b");
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->width(), 425);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->height(), 307);

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, ImageAddedDonut) {
    pdif::PDF pdf1("test_pdfs/image_no_image.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_change_image.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 6);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::xobject_image);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->image_hash(), "85740d00c5dfa840a9fb6c4c3ac47930150a3f49");
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->width(), 406);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->height(), 235);

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, ImageRemovedSpace) {
    pdif::PDF pdf1("test_pdfs/image_initial.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_no_image.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 6);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}
TEST(PDIFPDFCompare, ImageRemovedDonut) {
    pdif::PDF pdf1("test_pdfs/image_change_image.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_no_image.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 6);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, ImageChangedSpaceToDonut) {
    pdif::PDF pdf1("test_pdfs/image_initial.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_change_image.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 7);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::xobject_image);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->image_hash(), "85740d00c5dfa840a9fb6c4c3ac47930150a3f49");
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->width(), 406);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->height(), 235);

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(6);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, ImageChangedDonutToSpace) {
    pdif::PDF pdf1("test_pdfs/image_change_image.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_initial.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 7);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::xobject_image);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->image_hash(), "bc35317497c60e50332101c357ad9f2139dd728b");
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->width(), 425);
    ASSERT_EQ(op.get_arg()->as<pdif::xobject_img_elem>()->height(), 307);

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(6);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, ImageTextAdded) {
    pdif::PDF pdf1("test_pdfs/image_initial.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_added_text.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 8);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "See the image below:");

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(6);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Very cool.");

    op = d.get_edit_op(7);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}
TEST(PDIFPDFCompare, ImageTextRemoved) {
    pdif::PDF pdf1("test_pdfs/image_added_text.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/image_initial.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 8);
    
    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(4);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(5);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(6);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(7);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, AddedPageScopeDocument) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::document);
    pdif::PDF pdf2("test_pdfs/multi_page_added.pdf", pdif::granularity::sentence, pdif::scope::document);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 12);
    
    for (int i = 0; i < 9; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(9);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(10);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "This is the fourth page.");

    op = d.get_edit_op(11);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "4");
}

TEST(PDIFPDFCompare, AddedPageScopePage) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/multi_page_added.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 12);
    
    for (int i = 0; i < 9; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(9);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(10);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "This is the fourth page.");

    op = d.get_edit_op(11);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "4");
}

TEST(PDIFPDFCompare, RemovedPageScopeDocument) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::document);
    pdif::PDF pdf2("test_pdfs/multi_page_removed.pdf", pdif::granularity::sentence, pdif::scope::document);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 9);
    
    for (int i = 0; i < 6; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(6);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(7);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(8);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);
}

TEST(PDIFPDFCompare, RemovedPageScopePage) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/multi_page_removed.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 9);
    
    for (int i = 0; i < 6; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(6);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(7);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(8);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);
}

TEST(PDIFPDFCompare, InsertedPageScopeDocument) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::document);
    pdif::PDF pdf2("test_pdfs/multi_page_inserted.pdf", pdif::granularity::sentence, pdif::scope::document);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 13);

    for (int i = 0; i < 6; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(6);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(7);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "This is an inserted page.");

    op = d.get_edit_op(8);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "3");

    op = d.get_edit_op(9);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(10);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(11);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "4");
    
    op = d.get_edit_op(12);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);
}

TEST(PDIFPDFCompare, InsertedPageScopePage) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::page);
    pdif::PDF pdf2("test_pdfs/multi_page_inserted.pdf", pdif::granularity::sentence, pdif::scope::page);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 13);

    for (int i = 0; i < 7; i++) {
        auto op = d.get_edit_op(i);
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    }

    auto op = d.get_edit_op(7);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "This is an inserted page.");

    op = d.get_edit_op(8);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = d.get_edit_op(9);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(10);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::font_set);
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_name(), "CMR10");
    ASSERT_EQ(op.get_arg()->as<pdif::font_elem>()->font_size(), 9);

    op = d.get_edit_op(11);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "This is the third page.");
    
    op = d.get_edit_op(12);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "4");
}

TEST(PDIFPDFCompare, Pageno2) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::page, true, 1);
    pdif::PDF pdf2("test_pdfs/multi_page_2_text_add.pdf", pdif::granularity::sentence, pdif::scope::page, true, 1);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 4);

    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Hello World.");

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFPDFCompare, Pageno3) {
    pdif::PDF pdf1("test_pdfs/multi_page.pdf", pdif::granularity::sentence, pdif::scope::page, true, 2);
    pdif::PDF pdf2("test_pdfs/multi_page_3_text_add.pdf", pdif::granularity::sentence, pdif::scope::page, true, 2);

    pdif::diff d = pdf1.compare<pdif::lcs_stream_differ>(pdf2);

    ASSERT_EQ(d.edit_op_size(), 4);

    auto op = d.get_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(1);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);

    op = d.get_edit_op(2);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Hello World.");

    op = d.get_edit_op(3);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}