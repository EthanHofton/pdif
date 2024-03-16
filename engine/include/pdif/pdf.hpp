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
     * @param write_console_colors flag to set whether to write console colors (default: true)
     * @param pageno the page number to extract STARTING FROM 0 (default: -1 for all)
     */
    PDF(const std::string& path, granularity g = granularity::word, scope s = scope::page, bool write_console_colors = true, int pageno = -1);

    /**
     * @brief Get the granularity object
     * 
     * @return granularity 
     */
    inline granularity get_granularity() const { return m_extractor_granularity; }
    /**
     * @brief get the scope
     * 
     * @return scope
     */
    inline scope get_scope() const { return m_pdf_scope; }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<stream_differ_base, T>>>
    diff compare(const PDF& other) const {
        pdif::diff d(m_write_console_colors);

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
     * @param spacing the spacing to use (default: none)
     * 
     */
    void dump_content(std::ostream& t_out, std::optional<std::string> spacing = std::nullopt) const;

    /**
     * @brief Flag to set whether to write console colors
     * 
     * @param b the flag
     */
    inline void write_console_colors(bool b) { m_write_console_colors = b; }
    /**
     * @brief flag to check whether to write console colors
     * 
     * @return true
     * @return false 
     */
    inline bool write_console_colors() const { return m_write_console_colors; }

    /**
     * @brief Get the meta object
     * 
     * @return pdif::stream_meta 
     */
    pdif::stream_meta get_meta() const { return m_meta; }
    /**
     * @brief Get the streams object
     * 
     * @return std::vector<stream> 
     */
    std::vector<stream> get_streams() const { return m_streams; }

private:

    /**
     * @brief get a console color code (if enabled)
     * 
     * @param code the code
     * @return std::string returned color code
     */
    std::string cc(util::CONSOLE_COLOR_CODE code) const;

private:

    granularity m_extractor_granularity;
    scope m_pdf_scope;
    std::shared_ptr<QPDF> m_pdf;

    std::vector<stream> m_streams;
    pdif::stream_meta m_meta;

    bool m_write_console_colors;
    int m_pageno;
};

}

#endif // __PDIF_PDF_HPP__