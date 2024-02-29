#include <gtest/gtest.h>
#include <pdif/content_extractor.hpp>
#include <qpdf/QPDF.hh>

TEST(PDIFContentExtractor, TestExtractMeta) {
    QPDF pdf;
    pdf.processFile("test_pdfs/metadata_initial.pdf");

    pdif::stream_meta meta = pdif::extract_meta(pdf);

    ASSERT_EQ(meta.get_metadata("Keywords"), "Test Keywords");
    ASSERT_EQ(meta.get_metadata("Author"), "Test Author");
    ASSERT_EQ(meta.get_metadata("Subject"), "Test Subject");
    ASSERT_EQ(meta.get_metadata("Title"), "Test Title");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}