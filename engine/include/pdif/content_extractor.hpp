#ifndef __PDIF_CONTENT_EXTRACTOR_HPP__
#define __PDIF_CONTENT_EXTRACTOR_HPP__

#include <vector>
#include <pdif/stream.hpp>
#include <pdif/stream_meta.hpp>

#include <qpdf/QPDF.hh>
#include <qpdf/QPDFObjectHandle.hh>

namespace pdif {

/**
 * @brief An enum class to represent the granularity to use while extracting content
 * 
 * letter: the extraction is done at the letter level
 * word: the extraction is done at the word level
 * sentence: the extraction is done at the sentence level
 * object: the extraction is done at the PDF object level
 */
enum class granularity {
    letter,
    word,
    sentence,
    object
};

/**
 * @brief An enum class to represent the scope of the extraction
 * 
 * page: the extraction is limited to the current page, a list of streams are returned
 * document: the extraction is done on the whole document, a single stream is returned
 */
enum class scope {
    page,
    document
};

extern pdif::stream_meta extract_meta(QPDF& pdf);

}

#endif // __PDIF_CONTENT_EXTRACTOR_HPP__