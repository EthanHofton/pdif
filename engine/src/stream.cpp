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


} // namespace pdif