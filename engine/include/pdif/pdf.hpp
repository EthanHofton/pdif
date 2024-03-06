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

    struct comparison_args {
        bool compare_meta = false;
        bool compare_text = false;
        bool compare_image = false;
    };

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

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<stream_differ_base, T>>>
    diff compare(const PDF& other, comparison_args) const {
        pdif::diff d;

        // compare the meta
        stream_differ_base::meta_diff(d, m_meta, other.m_meta);

        int m = m_streams.size();
        int n = other.m_streams.size();

        // compare the streams
        for (int i = 0; i < std::max(m, n); i++) {
            if (i < m && i < n) {
                T differ(m_streams[i], other.m_streams[i]);
                d.add_original_stream(m_streams[i]);
                differ.diff(d);
            } else if (i < m) {
                d.add_original_stream(m_streams[i]);
                T differ(m_streams[i], stream());
                differ.diff(d);
            } else if (i < n) {
                T differ(stream(), other.m_streams[i]);
                differ.diff(d);
            }
        }
        
        return d;
    }

    /**
     * @brief Dump the meta data of the PDF to the output stream
     * 
     * @param t_out the output stream
     */
    void dump_meta(std::ostream&) const;
    /**
     * @brief Dump the content of the PDF to the output stream
     * 
     * @param t_out the output stream
     * @param pageno the page number to dump (negative for all, 0 for meta)
     */
    void dump_content(std::ostream&, int pageno = -1, std::optional<std::string> = std::nullopt) const;

private:

    granularity m_extractor_granularity;
    scope m_pdf_scope;
    std::shared_ptr<QPDF> m_pdf;

    std::vector<stream> m_streams;
    pdif::stream_meta m_meta;
};

}

#endif // __PDIF_PDF_HPP__