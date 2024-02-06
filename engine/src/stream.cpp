#include <pdif/stream.hpp>

namespace pdif {

void stream::check_index_read(size_t index) const {
    if (index >= size()) {
        PDIF_LOG_ERROR("Index out of bounds: {}", index);
        throw pdif_out_of_bounds("Index out of bounds: " + std::to_string(index));
    }
}

void stream::check_index_write(size_t index) const {
    if (index > size()) {
        PDIF_LOG_ERROR("Index out of bounds: {}", index);
        throw pdif_out_of_bounds("Index out of bounds: " + std::to_string(index));
    }
}

rstream_elem& stream::operator[](size_t index) {
    check_index_read(index);
    return elems[index];
}

const rstream_elem& stream::operator[](size_t index) const {
    check_index_read(index);
    return elems[index];
}

void stream::push_back(const rstream_elem& elem) {
    elems.push_back(elem);
}

void stream::push_front(const rstream_elem& elem) {
    elems.insert(elems.begin(), elem);
}

void stream::push(size_t index, const rstream_elem& elem) {
    check_index_write(index);
    elems.insert(elems.begin() + index, elem);
}

void stream::pop_back() {
    check_index_read(size() - 1);
    elems.pop_back();
}

void stream::pop_front() {
    check_index_read(0);
    elems.erase(elems.begin());
}

void stream::pop(size_t index) {
    check_index_read(index);
    elems.erase(elems.begin() + index);
}

void stream::check_empty() const {
    if (empty()) {
        PDIF_LOG_ERROR("Cannot create iterator, stream is empty");
        throw pdif_out_of_bounds("Cannot create iterator, stream is empty");
    }
}

size_t stream::size() const {
    return elems.size();
}

bool stream::empty() const {
    return elems.empty();
}

void stream::clear() {
    elems.clear();
}

void stream::add_metadata(const std::string& t_key, const std::string& t_value) {
    if (has_key(t_key)) {
        PDIF_LOG_ERROR("stream::add_metadata: Key already exists: {}", t_key);
        throw pdif::pdif_invalid_key("stream::add_metadata: Key already exists: " + t_key);
    }

    m_metadata.emplace(t_key, t_value);
}

void stream::remove_metadata(const std::string& t_key) {
    if (!has_key(t_key)) {
        PDIF_LOG_ERROR("stream::remove_metadata: Key does not exist: {}", t_key);
        throw pdif::pdif_invalid_key("stream::remove_metadata: Key does not exist: " + t_key);
    }

    m_metadata.erase(t_key);
}

void stream::update_metadata(const std::string& t_key, const std::string& t_value) {
    if (!has_key(t_key)) {
        PDIF_LOG_ERROR("stream::update_metadata: Key does not exist: {}", t_key);
        throw pdif::pdif_invalid_key("stream::update_metadata: Key does not exist: " + t_key);
    }

    m_metadata[t_key] = t_value;
}

std::string stream::get_metadata(const std::string& t_key) const {
    if (!has_key(t_key)) {
        PDIF_LOG_ERROR("stream::get_metadata: Key does not exist: {}", t_key);
        throw pdif::pdif_invalid_key("stream::get_metadata: Key does not exist: " + t_key);
    }

    return m_metadata.at(t_key);
}

bool stream::has_key(const std::string& t_key) const {
    if (m_metadata.find(t_key) == m_metadata.end()) {
        return false;
    }

    return true;
}

void stream::stream_callback(const pdif::edit_op& t_op) const {
    if (!has_stream_callback()) {
        PDIF_LOG_ERROR("stream::stream_callback: Stream callback not set");
        throw pdif::pdif_invalid_operation("stream::stream_callback: Stream callback not set");
    }

    try {
        m_stream_callback.value()(std::forward<const pdif::edit_op&>(t_op));
    } catch (const std::exception& e) {
        PDIF_LOG_ERROR("stream::stream_callback error in callback: {}", e.what());
        throw pdif::pdif_error_in_callback("stream::stream_callback error in callback: " + std::string(e.what()));
    }
}

void stream::meta_callback(const pdif::meta_edit_op& t_op) const {
    if (!has_meta_callback()) {
        PDIF_LOG_ERROR("stream::meta_callback: Meta callback not set");
        throw pdif::pdif_invalid_operation("stream::meta_callback: Meta callback not set");
    }

    try {
        m_meta_callback.value()(std::forward<const pdif::meta_edit_op&>(t_op));
    } catch (const std::exception& e) {
        PDIF_LOG_ERROR("stream::meta_callback error in callback: {}", e.what());
        throw pdif::pdif_error_in_callback("stream::meta_callback error in callback: " + std::string(e.what()));
    }
}

} // namespace pdif