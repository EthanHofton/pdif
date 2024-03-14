#ifndef __HPP_PDF_CONTENT_STREAM_FILTER__
#define __HPP_PDF_CONTENT_STREAM_FILTER__

#include <pdif/stream.hpp>
#include <pdif/content_extractor.hpp>
#include <qpdf/QPDF.hh>
#include <qpdf/QPDFObjectHandle.hh>

namespace pdif {


/**
 * @brief A class to parse through a PDF content stream and abstract the PDF as a series of stream_elems
 * 
 */
class pdf_content_stream_filter : public QPDFObjectHandle::TokenFilter {
public:

    pdf_content_stream_filter(stream& s, granularity g, QPDFObjectHandle root) : m_stream(s), m_g(g), m_root(root) {}
    ~pdf_content_stream_filter() override = default;

    /**
     * @brief Handel a token in the content stream
     * 
     * @param token the token to handle
     */
    void handleToken(QPDFTokenizer::Token const& token) override;
    /**
     * @brief Handel and EOF token
     * 
     */
    void handleEOF() override;

private:

    // handlers
    /**
     * @brief Handel what happens when an operator is encountered
     * 
     * @param token the Operator token
     */
    void handleOperator(QPDFTokenizer::Token const& token);
    /**
     * @brief Handel what happens when a string write operator is encountered
     * 
     */
    void handleStringWrite();
    /**
     * @brief Handle font changes (update some state variables)
     * 
     */
    void handleFontChange();
    /**
     * @brief Handel what happens when a text color is set
     * 
     */
    void handleTextColorSet();
    /**
     * @brief Handel what happens when a stroke color is set
     * 
     */
    void handleStrokeColorSet();
    /**
     * @brief Handel what happens when an XObject is displayed
     * 
     */
    void handleXObject();

    /**
     * @brief Hash an image buffer
     * 
     * @param data the data
     * @param size the size
     */
    std::string imageToHash(const unsigned char* data, size_t size);

    /**
     * @brief Flush the string buffer
     * 
     */
    void flushStringBuffer();

    /**
     * @brief parse the cmap into the current font
     * 
     * @param cmap 
     */
    void parseCMap(const std::string& cmap);

    /**
     * @brief Get the Post Script Font Encoding for a postscript font
     * 
     * @param postscript_font 
     */
    void getPostScriptFontEncoding(const std::string& postscript_font);
    
    /**
     * @brief do unicode normalization on the input string
     * 
     * @param str 
     * @return std::string 
     */
    std::string unicodeNormalize(const std::string& str) const;

private:

    // single arg, or array arg
    using arg_type = std::variant<QPDFTokenizer::Token, std::vector<QPDFTokenizer::Token>>;

    struct arg_visitor {
        std::string operator()(QPDFTokenizer::Token const& t);
        std::string operator()(std::vector<QPDFTokenizer::Token> const& t);
        std::optional<pdif::rfont_elem> current_font;
    };

    struct state {
        bool in_array = false;

        // current state of the stream
        std::optional<pdif::rfont_elem> current_font;
        std::optional<pdif::rtext_color_elem> current_text_color;
        std::optional<pdif::rstroke_color_elem> current_stroke_color;
    };

    stream& m_stream;
    granularity m_g;
    QPDFObjectHandle m_root;
    std::string m_string_buffer;

    state m_state;

    std::vector<arg_type> m_arg_stack;

    static constexpr double SPACE_THRESHOLD = -70;
};

};

#endif // __HPP_PDF_CONTENT_STREAM_FILTER__