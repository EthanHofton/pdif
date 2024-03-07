#include <pdif/content_extractor.hpp>
#include <pdif/pdf_content_stream_filter.hpp>
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

extern std::vector<pdif::stream> extract_content(std::shared_ptr<QPDF> pdf, granularity g, scope s) {
    using T_filter = pdf_content_stream_filter;
    std::vector<pdif::stream> streams;

    std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(*pdf).getAllPages();

    if (s == scope::document) {
        streams.push_back(pdif::stream());
    }

    for (auto& page : pages) {
        if (s == scope::page) {
            pdif::stream s = pdif::stream();
            T_filter tf(s, g);

            page.filterContents(&tf);
            streams.push_back(s);
        } else if (s == scope::document) {
            T_filter tf(streams[0], g);

            page.filterContents(&tf);
        }
    }

    return streams;
}

}