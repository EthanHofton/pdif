#include <pdif/stream_elem.hpp>
#include <pdif/errors.hpp>

namespace pdif {

stream_elem::stream_elem(private_tag, stream_type t_type) : m_type(t_type){}
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

std::string text_elem::to_string(bool) const {
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

std::string font_elem::to_string(bool console_colors) const {
    std::stringstream ss;
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_BLUE, console_colors);
    ss << "[Font set: " << m_font_name << ", " << m_font_size << "pt]";
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT, console_colors);
    return ss.str();
}

// ** ====== TEXT COLOR ELEM ====== ** //
text_color_elem::text_color_elem(stream_elem::private_tag t, float t_r, float t_g, float t_b) :
    color_elem(t, stream_type::text_color_set, t_r, t_g, t_b) {}

bool text_color_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::text_color_set) {
        return false;
    }

    auto other = t_other->as<text_color_elem>();
    return r == other->red() && g == other->green() && b == other->blue();
}

std::string text_color_elem::to_string(bool console_colors) const {
    std::stringstream ss;
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_BLUE, console_colors);
    ss << "[Text color set: " << r << "r, " << g << "g, " << b << "b]";
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT, console_colors);
    return ss.str();
}

// ** ====== STROKE COLOR ELEM ====== ** //
stroke_color_elem::stroke_color_elem(stream_elem::private_tag t, float t_r, float t_g, float t_b) :
    color_elem(t, stream_type::stroke_color_set, t_r, t_g, t_b) {}

bool stroke_color_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::stroke_color_set) {
        return false;
    }

    auto other = t_other->as<stroke_color_elem>();
    return r == other->red() && g == other->green() && b == other->blue();
}

std::string stroke_color_elem::to_string(bool console_colors) const {
    std::stringstream ss;
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_BLUE, console_colors);
    ss << "[Stroke color set: " << r << "r, " << g << "g, " << b << "b]";
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT, console_colors);
    return ss.str();
}

// ** ====== XOBJECT ELEM ====== ** //

xobject_img_elem::xobject_img_elem(stream_elem::private_tag t, const std::string& t_hash, int t_width, int t_height) :
    stream_elem(t, stream_type::xobject_image),
    m_image_hash(t_hash),
    m_width(t_width),
    m_height(t_height) {}

bool xobject_img_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::xobject_image) {
        return false;
    }

    auto other = t_other->as<xobject_img_elem>();
    return m_image_hash == other->image_hash() && m_width == other->width() && m_height == other->height();
};

std::string xobject_img_elem::to_string(bool console_colors) const {
    std::stringstream ss;
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_LIGHT_MAGENTA, console_colors);
    ss << "[Image: (sha1)" << m_image_hash << ", " << m_width << "x" << m_height << "px]";
    ss << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET, console_colors);
    ss << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT, console_colors);
    return ss.str();
}

} // namespace pdif