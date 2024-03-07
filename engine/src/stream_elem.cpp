#include <pdif/stream_elem.hpp>
#include <pdif/errors.hpp>

namespace pdif {

stream_elem::stream_elem(private_tag, stream_type t_type) : m_type(t_type) {}
stream_type stream_elem::type() const { return m_type; }

text_elem::text_elem(stream_elem::private_tag t, const std::string& t_text) :
    stream_elem(t, stream_type::text),
    m_text(t_text) {}

const std::string& text_elem::text() const {
    return m_text;
}

bool text_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::text) {
        return false;
    }

    return m_text == t_other->as<text_elem>()->text();
}

std::string text_elem::to_string() const {
    return m_text;
}

// ** ====== FONT ELEM ====== ** //

font_elem::font_elem(stream_elem::private_tag t, const std::string& t_font_name, int t_font_size) :
    stream_elem(t, stream_type::font_set),
    m_font_name(t_font_name),
    m_font_size(t_font_size) {}

const std::string& font_elem::font_name() const {
    return m_font_name;
}

int font_elem::font_size() const {
    return m_font_size;
}

bool font_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::font_set) {
        return false;
    }

    auto other = t_other->as<font_elem>();
    return m_font_name == other->font_name() && m_font_size == other->font_size();
}

std::string font_elem::to_string() const {
    std::stringstream ss;
    ss << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    ss << util::CONSOLE_COLOR_CODE::FG_BLUE;
    ss << "[Font set: " << m_font_name << ", " << m_font_size << "pt]";
    ss << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    ss << util::CONSOLE_COLOR_CODE::FG_DEFAULT;
    return ss.str();
}

// ** ====== TEXT COLOR ELEM ====== ** //
text_color_elem::text_color_elem(stream_elem::private_tag t, int t_r, int t_g, int t_b) :
    color_elem(t, stream_type::text_color_set, t_r, t_g, t_b) {}

bool text_color_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::text_color_set) {
        return false;
    }

    auto other = t_other->as<text_color_elem>();
    return r == other->red() && g == other->green() && b == other->blue();
}

std::string text_color_elem::to_string() const {
    std::stringstream ss;
    ss << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    ss << util::CONSOLE_COLOR_CODE::FG_BLUE;
    ss << "[Text color set: " << r << "r, " << g << "g, " << b << "b]";
    ss << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    ss << util::CONSOLE_COLOR_CODE::FG_DEFAULT;
    return ss.str();
}

// ** ====== STROKE COLOR ELEM ====== ** //
stroke_color_elem::stroke_color_elem(stream_elem::private_tag t, int t_r, int t_g, int t_b) :
    color_elem(t, stream_type::stroke_color_set, t_r, t_g, t_b) {}

bool stroke_color_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::stroke_color_set) {
        return false;
    }

    auto other = t_other->as<stroke_color_elem>();
    return r == other->red() && g == other->green() && b == other->blue();
}

std::string stroke_color_elem::to_string() const {
    std::stringstream ss;
    ss << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    ss << util::CONSOLE_COLOR_CODE::FG_BLUE;
    ss << "[Stroke color set: " << r << "r, " << g << "g, " << b << "b]";
    ss << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    ss << util::CONSOLE_COLOR_CODE::FG_DEFAULT;
    return ss.str();
}

} // namespace pdif