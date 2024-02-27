#include <pdif/pdf.hpp>

namespace pdif {

PDF::PDF(const std::string& path, granularity g, scope s) {
    m_pdf = QPDF();
    m_pdf.processFile(path.c_str());

    m_extractor_granularity = g;
    m_pdf_scope = s;

    m_meta = extract_meta(m_pdf);
    // m_streams = extract_content(m_pdf, m_extractor_granularity, m_pdf_scope);
}

pdif::diff PDF::compare(const PDF& other const {
    pdif::diff d;
}

}