#include <gtest/gtest.h>
#include <pdif/content_extractor.hpp>
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

TEST(PDIFContentExtractor, TestExtractContent) {
    std::shared_ptr<QPDF> pdf;
    pdf->processFile("test_pdfs/metadata_initial.pdf");

    std::vector<pdif::stream> streams = pdif::extract_content(pdf, pdif::granularity::word, pdif::scope::page);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}