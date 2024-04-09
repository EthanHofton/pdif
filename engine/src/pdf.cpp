#include <pdif/pdf.hpp>

namespace pdif {

PDF::PDF(const std::string& path, granularity g, scope s, bool write_console_colors, int pageno, bool allow_state_set_nochange) : m_extractor_granularity(g), m_pdf_scope(s), m_write_console_colors(write_console_colors), m_pageno(pageno) {
    m_pdf = QPDF::create();

    m_pdf->processFile(path.c_str());

    m_meta = extract_meta(m_pdf);
    m_streams = extract_content(m_pdf, m_extractor_granularity, m_pdf_scope, m_pageno, allow_state_set_nochange);
}

std::string PDF::cc(util::CONSOLE_COLOR_CODE code) const {
    if (m_write_console_colors) {
        std::stringstream ss;
        ss << code;
        return ss.str();
    }

    return "";
}

void PDF::dump_meta(std::ostream& t_out) const {
    t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "Meta:" << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;
    t_out << std::endl;

    for (auto& [key, value] : m_meta.get_metadata()) {
        t_out << key << ": " << value << std::endl;
    }

    t_out << std::endl;
    t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "== Meta Finished ==" << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;
}

void PDF::dump_content(std::ostream& t_out, std::optional<std::string> spacing) const {
    for (size_t y = 0; y < m_streams.size(); y++) {
        if (m_pageno >= 0) {
            t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "Page " << m_pageno + 1<< ":";
        } else {
            t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "Page " << y + 1 << ":";
        }

        t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;
        t_out << std::endl;
        auto stream = m_streams[y];
        for (size_t i = 0; i < stream.size(); i++) {
            t_out << stream[i]->to_string(m_write_console_colors);

            if (spacing.has_value()) {
                t_out << spacing.value();
            } else {
                switch (m_extractor_granularity) {
                    case granularity::letter:
                        break;
                    case granularity::sentence:
                        t_out << " ";
                        break;
                    case granularity::word:
                        t_out << " ";
                        break;
                }
            }
        }
        t_out << std::endl;
        if (m_pageno >= 0) {
            t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "== Page " << m_pageno + 1 << " Finished ==";
        } else {
            t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "== Page " << y + 1 << " Finished ==";
        }
        t_out << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;
    }
}

}