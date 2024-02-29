#include <pdif/pdf.hpp>

namespace pdif {

PDF::PDF(const std::string& path, granularity g, scope s) : m_pdf(QPDF()) {
    m_pdf.processFile(path.c_str());

    m_extractor_granularity = g;
    m_pdf_scope = s;

    m_meta = extract_meta(m_pdf);
    // m_streams = extract_content(m_pdf, m_extractor_granularity, m_pdf_scope);
}

pdif::diff PDF::compare(const PDF& other, pdif::PDF::comparison_options op) const {
    pdif::diff d;

    // compare the meta
    stream_differ_base::meta_diff(d, m_meta, other.m_meta);

    if (op == comparison_options::meta_only) {
        return d;
    }

    // compare the streams
    if (m_streams.size() != other.m_streams.size()) {
        // TODO:
    } else {
        for (size_t i = 0; i < m_streams.size(); i++) {
            // stream_differ_base::stream_diff(d, m_streams[i], other.m_streams[i]);
        }
    }



    return d;
}

}