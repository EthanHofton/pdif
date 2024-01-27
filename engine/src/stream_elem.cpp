#include <pdif/stream_elem.hpp>
#include <pdif/errors.hpp>

namespace pdif {

stream_elem::stream_elem(private_tag t, stream_type t_type) : m_type(t_type) {}
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

binary_elem::binary_elem(stream_elem::private_tag t, const std::vector<char>& t_binary) :
    stream_elem(t, stream_type::binary),
    m_binary(t_binary) {}

const std::vector<char>& binary_elem::binary() const {
    return m_binary;
}

bool binary_elem::compare(rstream_elem t_other) {
    if (t_other->type() != stream_type::binary) {
        return false;
    }

    return m_binary == t_other->as<binary_elem>()->binary();
}

}