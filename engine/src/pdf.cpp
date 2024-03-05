#include <pdif/pdf.hpp>

namespace pdif {

PDF::PDF(const std::string& path, granularity g, scope s) {
    m_pdf = QPDF::create();

    m_pdf->processFile(path.c_str());

    m_extractor_granularity = g;
    m_pdf_scope = s;

    m_meta = extract_meta(m_pdf);
    m_streams = extract_content(m_pdf, m_extractor_granularity, m_pdf_scope);
}

void PDF::dump_meta(std::ostream& t_out) const {
    t_out << util::CONSOLE_COLOR_CODE::TEXT_BOLD << "Meta:" << util::CONSOLE_COLOR_CODE::TEXT_RESET << std::endl;
    t_out << std::endl;

    for (auto& [key, value] : m_meta.get_metadata()) {
        t_out << key << ": " << value << std::endl;
    }

    t_out << std::endl;
    t_out << util::CONSOLE_COLOR_CODE::TEXT_BOLD << "== Meta Finished ==" << util::CONSOLE_COLOR_CODE::TEXT_RESET << std::endl;
}

void PDF::dump_content(std::ostream& t_out, int pageno, std::optional<std::string> spacing) const {
    if (pageno == 0) {
        dump_meta(t_out);
    } else if (pageno < 0) {
        // all
        for (size_t y = 0; y < m_streams.size(); y++) {
            t_out << util::CONSOLE_COLOR_CODE::TEXT_BOLD << "Page " << y + 1 << ":" << util::CONSOLE_COLOR_CODE::TEXT_RESET << std::endl;
            t_out << std::endl;
            auto stream = m_streams[y];
            for (size_t i = 0; i < stream.size(); i++) {
                t_out << stream[i]->to_string();

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
            t_out << util::CONSOLE_COLOR_CODE::TEXT_BOLD << "== Page " << y + 1 << " Finished ==" << util::CONSOLE_COLOR_CODE::TEXT_RESET << std::endl;
        }
    } else {
        // single
        if ((size_t)pageno > m_streams.size()) {
            throw std::runtime_error("Page number out of range");
        }

        t_out << util::CONSOLE_COLOR_CODE::TEXT_BOLD << "Page " << pageno << ":" << util::CONSOLE_COLOR_CODE::TEXT_RESET << std::endl;
        t_out << std::endl;
        auto stream = m_streams[pageno - 1];
        for (size_t i = 0; i < stream.size(); i++) {
            t_out << stream[i]->to_string();

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
        t_out << util::CONSOLE_COLOR_CODE::TEXT_BOLD << "== Page " << pageno << " Finished ==" << util::CONSOLE_COLOR_CODE::TEXT_RESET << std::endl;
    }
}

}