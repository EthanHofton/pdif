#include <pdif/content_extractor.hpp>
#include <vector>

namespace pdif {

extern pdif::stream_meta extract_meta(QPDF& pdf) {
    pdif::stream_meta meta;

    // Access the document information dictionary
    QPDFObjectHandle trailer = pdf.getTrailer();

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

    token_filter(stream& s) : m_stream(s) {}
    ~token_filter() override = default;

    void handleToken(QPDFTokenizer::Token const& token) override {
        // reconstruct the character stream
        // PDF will break up strings into substrings
        // check distance between tokens to reconstruct the string

        if (token.getType() == QPDFTokenizer::tt_string) {
            m_stream.push_back(pdif::stream_elem::create<pdif::text_elem>(token.getValue()));
        }

        if (token.getType() == QPDFTokenizer::tt_inline_image) {
            // TODO:
        }

        writeToken(token);
    }

    void handleEOF() override {}

private:

    pdif::stream& m_stream;
};

extern std::vector<pdif::stream> extract_content(QPDF& pdf, granularity, scope s) {
    std::vector<pdif::stream> streams;

    std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(pdf).getAllPages();

    if (s == scope::document) {
        streams.push_back(pdif::stream());
    }

    for (auto& page : pages) {
        if (s == scope::page) {
            pdif::stream s = pdif::stream();
            token_filter tf(s);

            page.filterContents(&tf);

            streams.push_back(s);
        } else if (s == scope::document) {
            token_filter tf(streams[0]);

            page.filterContents(&tf);
        }
    }

    return streams;
}

}