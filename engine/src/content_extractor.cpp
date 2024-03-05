#include <pdif/content_extractor.hpp>
#include <vector>

namespace pdif {

extern pdif::stream_meta extract_meta(std::shared_ptr<QPDF> pdf) {
    pdif::stream_meta meta;

    // Access the document information dictionary
    QPDFObjectHandle trailer = pdf->getTrailer();

    if (trailer.hasKey("/Info")) {
        for (auto& it : trailer.getKey("/Info").ditems()) {
            std::string val;
            if (it.second.isString()) {
                val = it.second.getStringValue();
            } else if (it.second.isName()) {
                val = it.second.getName();
            } else {
                val = it.second.unparseResolved();
            }

            // remove / from the key
            meta.add_metadata(it.first.substr(1), val);
        }
    }

    return meta;
}

class token_filter : public QPDFObjectHandle::TokenFilter {
public:

    token_filter(stream& s, granularity g) : m_stream(s), m_g(g) {}
    ~token_filter() override = default;

    void handleToken(QPDFTokenizer::Token const& token) override {
        if (prev_token.has_value()) {
            if (prev_token.value().getType() == QPDFTokenizer::tt_string) {
                if (token.getType() == QPDFTokenizer::tt_integer || token.getType() == QPDFTokenizer::tt_real) {
                    if (std::stoi(token.getValue()) < -50) {
                        current_string.push_back(' ');
                    }
                }
            }
        }

        if (token.getType() == QPDFTokenizer::tt_string) {
            current_string.append(token.getValue());
        }

        if (token.getType() == QPDFTokenizer::tt_word) {
            if (token.getValue() == "TJ" || token.getValue() == "Tj") {
                string_buffer.append(current_string);
                current_string.clear();
                
                switch (m_g) {
                    case granularity::letter:
                        for (auto c : string_buffer) {
                            if (c == ' ') {
                                continue;
                            }
                            std::string s;
                            s.push_back(c);
                            m_stream.push_back(stream_elem::create<text_elem>(s));
                        }
                        string_buffer.clear();
                        break;
                    case granularity::word: {
                        std::istringstream iss(string_buffer);
                        std::string word;
                        while (iss >> word) {
                            m_stream.push_back(stream_elem::create<text_elem>(word));
                        }
                        string_buffer.clear();
                        break;
                    }
                    case granularity::sentence: {
                        size_t fullstop_pos = string_buffer.find('.');
                        while (fullstop_pos != std::string::npos) {
                            std::string sentence = string_buffer.substr(0, fullstop_pos + 1);
                            m_stream.push_back(stream_elem::create<text_elem>(sentence));
                            string_buffer.erase(0, fullstop_pos + 1);
                            fullstop_pos = string_buffer.find('.');
                        }
                        break;
                    }
                        

                }
            } else {
                current_string.clear();
            }
        }

        prev_token = token;
        writeToken(token);
    }

    void handleEOF() override {}

    void dealDanglingString() {
        if (string_buffer.size() > 0) {
            m_stream.push_back(stream_elem::create<text_elem>(string_buffer));
        }
    }

private:

    std::optional<QPDFTokenizer::Token> prev_token;
    std::string current_string;
    std::string string_buffer;
    pdif::stream& m_stream;
    granularity m_g;
};

extern std::vector<pdif::stream> extract_content(std::shared_ptr<QPDF> pdf, granularity g, scope s) {
    std::vector<pdif::stream> streams;

    std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(*pdf).getAllPages();

    if (s == scope::document) {
        streams.push_back(pdif::stream());
    }

    for (auto& page : pages) {
        if (s == scope::page) {
            pdif::stream s = pdif::stream();
            token_filter tf(s, g);

            page.filterContents(&tf);
            tf.dealDanglingString();
            streams.push_back(s);
        } else if (s == scope::document) {
            token_filter tf(streams[0], g);

            page.filterContents(&tf);
            tf.dealDanglingString();
        }
    }

    return streams;
}

}