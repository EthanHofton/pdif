#ifndef __PDIF_PDF_HPP__
#define __PDIF_PDF_HPP__

#include <pdif/stream.hpp>
#include <pdif/diff.hpp>
#include <pdif/stream_differ_base.hpp>
#include <pdif/content_extractor.hpp>

#include <qpdf/QPDF.hh>

namespace pdif {

/**
 * @brief A class to represent a PDF object
 * 
 */
class PDF {
public:

    /**
     * @brief Construct a new PDF object
     * 
     * @param path the path of the PDF file
     * @param g the granularity of the extractor (default: word)
     * @param s the scope of the extractor (default: page)
     */
    PDF(const std::string& path, granularity g = granularity::word, scope s = scope::page);

    /**
     * @brief Get the granularity object
     * 
     * @return granularity 
     */
    inline granularity get_granularity() const { return m_extractor_granularity; }
    /**
     * @brief Set the scope object
     * 
     * @param s the scope to set
     */
    inline scope get_scope() const { return m_pdf_scope; }

    diff compare(const PDF& other) const;

private:

    granularity m_extractor_granularity;
    scope m_pdf_scope;
    QPDF m_pdf;

    std::vector<stream> m_streams;
    pdif::stream_meta m_meta;
};

}

#endif // __PDIF_PDF_HPP__