#include <pdif/content_extractor.hpp>

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

}