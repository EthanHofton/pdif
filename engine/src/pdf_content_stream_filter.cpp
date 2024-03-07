#include <pdif/pdf_content_stream_filter.hpp>

namespace pdif {

std::string pdf_content_stream_filter::arg_visitor::operator()(QPDFTokenizer::Token const& t) {
    return t.getValue();
}

std::string pdf_content_stream_filter::arg_visitor::operator()(std::vector<QPDFTokenizer::Token> const& t) {
    std::string s;

    for (auto& token : t) {
        if (token.getType() == QPDFTokenizer::tt_string) {
            s.append(token.getValue());
        } else if (token.getType() == QPDFTokenizer::tt_integer || token.getType() == QPDFTokenizer::tt_real) {
            if (std::stoi(token.getValue()) < SPACE_THRESHOLD) {
                s.push_back(' ');
            }
        }
    }

    return s;
}

void pdf_content_stream_filter::handleToken(QPDFTokenizer::Token const& token) {
    auto type = token.getType();
    if (type == QPDFTokenizer::tt_word) {
        handleOperator(token);
        m_arg_stack.clear();
    } else if (
        type == QPDFTokenizer::tt_string ||
        type == QPDFTokenizer::tt_integer ||
        type == QPDFTokenizer::tt_real ||
        type == QPDFTokenizer::tt_name ||
        type == QPDFTokenizer::tt_bool ||
        type ==QPDFTokenizer::tt_inline_image
    ) {
        if (m_state.in_array) {
            try {
                std::get<std::vector<QPDFTokenizer::Token>>(m_arg_stack.back()).push_back(token);
            } catch (std::bad_variant_access const&) {
                throw std::runtime_error("Invalid array stack - expected arg array type but got single arg type");
            }
        } else {
            m_arg_stack.push_back(token);
        }
    } else if (type == QPDFTokenizer::tt_array_open) {
        m_state.in_array = true;
        m_arg_stack.push_back(std::vector<QPDFTokenizer::Token>());
    } else if (type == QPDFTokenizer::tt_array_close) {
        if (!m_state.in_array) {
            throw std::runtime_error("Unbalanced array - close found without open");
        }
        m_state.in_array = false;
    }
}

void pdf_content_stream_filter::handleOperator(QPDFTokenizer::Token const& token) {
    if (token.getValue() == "TJ" || token.getValue() == "Tj") {
        handleStringWrite();
    }

    if (token.getValue() == "Tf") {
        handleFontChange();
    }

    // if (token.getValue() == "")
}

void pdf_content_stream_filter::handleStringWrite() {
    for (auto& arg : m_arg_stack) {
        std::string val = std::visit(arg_visitor(), arg);
        m_string_buffer.append(val);
    }

    switch (m_g) {
        case granularity::letter:
            for (auto c : m_string_buffer) {
                if (c == ' ') {
                    continue;
                }

                std::string s;
                s.push_back(c);
                m_stream.push_back(stream_elem::create<text_elem>(s));
                m_string_buffer.clear();
            }
            break;
        case granularity::word: {
            std::istringstream iss(m_string_buffer);
            std::string word;
            while (iss >> word) {
                m_stream.push_back(stream_elem::create<text_elem>(word));
            }
            m_string_buffer.clear();
            break;
        }
        case granularity::sentence: {
            size_t fullstop_pos = m_string_buffer.find('.');
            while (fullstop_pos != std::string::npos) {
                std::string sentence = m_string_buffer.substr(0, fullstop_pos + 1);
                m_stream.push_back(stream_elem::create<text_elem>(sentence));
                m_string_buffer.erase(0, fullstop_pos + 1);
                fullstop_pos = m_string_buffer.find('.');
            }
            break;
        }
    }
}

void pdf_content_stream_filter::handleFontChange() {
    if (m_arg_stack.size() != 2) {
        throw std::runtime_error("Invalid font change - expected 2 args");
    }

    std::string font_name = std::visit(arg_visitor(), m_arg_stack[0]);
    int font_size = std::stoi(std::visit(arg_visitor(), m_arg_stack[1]));

    // extract the font name from the root
    QPDFObjectHandle resources = m_root.getKey("/Resources");
    QPDFObjectHandle font = resources.getKey("/Font");
    QPDFObjectHandle font_obj = font.getKey(font_name);

    if (!font_obj.isDictionary()) {
        throw std::runtime_error("Font not found");
    }

    font_name = font_obj.getKey("/BaseFont").getName();
    
    if (font_name.find("+") != std::string::npos) {
        font_name = font_name.substr(font_name.find("+") + 1);
    }

    m_stream.push_back(stream_elem::create<font_elem>(font_name, font_size));
}

void pdf_content_stream_filter::handleEOF() {
    if (m_string_buffer.size() > 0) {
        m_stream.push_back(stream_elem::create<text_elem>(m_string_buffer));
    }

    if (m_state.in_array) {
        throw std::runtime_error("Unbalanced array - EOF found");
    }
}

}