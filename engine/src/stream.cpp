#include <pdif/stream.hpp>

namespace pdif {

stream::iterator::iterator(pdif::stream& stream, size_t index) : m_stream(stream), m_index(index) {}

stream::iterator::reference stream::iterator::operator*() {
    return m_stream[m_index];
}

stream::iterator::value_type stream::iterator::operator->() {
    return m_stream[m_index];
}

stream::iterator& stream::iterator::operator++() {
    ++m_index;
    return *this;
}

stream::iterator stream::iterator::operator++(int) {
    auto copy = *this;
    ++m_index;
    return copy;
}

stream::iterator& stream::iterator::operator--() {
    --m_index;
    return *this;
}

stream::iterator stream::iterator::operator--(int) {
    auto copy = *this;
    --m_index;
    return copy;
}

bool stream::iterator::operator==(const stream::iterator& other) const {
    return m_index == other.m_index && &m_stream == &other.m_stream;
}

bool stream::iterator::operator!=(const stream::iterator& other) const {
    return !(*(this) == other);
}

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

stream::iterator stream::begin() {
    check_empty();
    return iterator(*this, 0);
}

stream::iterator stream::end() {
    check_empty();
    return iterator(*this, size() - 1);
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

} // namespace pdif