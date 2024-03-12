#ifndef __PDIF_CONTENT_EXTRACTOR_HPP__
#define __PDIF_CONTENT_EXTRACTOR_HPP__

#include <vector>
#include <pdif/stream.hpp>
#include <pdif/stream_meta.hpp>

#include <qpdf/QPDF.hh>
#include <qpdf/QPDFObjectHandle.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>
#include <sstream>

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

/**
 * @brief extract the metadata from a given PDF
 * 
 * @param pdf the PDF to extract the metadata from
 * @return pdif::stream_meta the metadata
 */
extern pdif::stream_meta extract_meta(std::shared_ptr<QPDF> pdf);

/**
 * @brief extract the content from a given PDF
 * 
 * @param pdf the PDF to extract the content from
 * @param g the granularity to use
 * @param s the scope to use
 * @return std::vector<pdif::stream> the extracted content
 */
extern std::vector<pdif::stream> extract_content(std::shared_ptr<QPDF> pdf, granularity g, scope s, int pageno = -1);

}

#endif // __PDIF_CONTENT_EXTRACTOR_HPP__