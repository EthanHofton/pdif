#ifndef __PDIF_STREAM_HPP__
#define __PDIF_STREAM_HPP__

#include "stream_elem.hpp"

#include <util/memory.hpp>
#include <pdif/logger.hpp>
#include <pdif/errors.hpp>

#include <algorithm>
#include <vector>
#include <map>
#include <functional>
#include <optional>

namespace pdif {

// edit_op forward declare
class edit_op;

/**
 * @brief a stream is a container for stream_elem objects
 * 
 */
class stream {
public:

    /**
     * @brief the callback function to be used when executing an edit operation
     * 
     */
    using stream_callback_f = std::function<void(const edit_op&)>;

    /**
     * @brief Construct a new stream object
     * 
     */
    stream() = default;
    /**
     * @brief Construct a new stream object
     * 
     */
    stream(const stream&) = default;

    /**
     * @brief index operator
     * 
     * @param index the index to access
     * @return rstream_elem& the element at the index
     */
    rstream_elem& operator[](size_t index);
    /**
     * @brief const index operator
     * 
     * @param index the index to access
     * @return const rstream_elem& const reference to the element at the index
     */
    const rstream_elem& operator[](size_t index) const;

    // element access
    /**
     * @brief push an element to the back of the stream
     * 
     * @param elem the element to push
     */
    void push_back(const rstream_elem& elem);
    /**
     * @brief push an element to the front of the stream
     * 
     * @param elem the element to push
     */
    void push_front(const rstream_elem& elem);
    /**
     * @brief push an element to the stream at the given index
     * 
     * @param index the index to push the element to
     * @param elem the element to push
     */
    void push(size_t index, const rstream_elem& elem);

    /**
     * @brief pop an element from the back of the stream
     * 
     */
    void pop_back();
    /**
     * @brief pop an element from the front of the stream
     * 
     */
    void pop_front();
    /**
     * @brief pop an element from the stream at the given index
     * 
     * @param index the index to pop the element from
     */
    void pop(size_t index);

    /**
     * @brief size of the stream
     * 
     * @return size_t the size of the stream
     */
    size_t size() const;
    /**
     * @brief checks if the stream is empty
     * 
     * @return true if the stream is empty
     * @return false otherwise
     */
    bool empty() const;

    /**
     * @brief create the stream
     * 
     */
    void clear();

    /**
     * @brief Set the stream callback object
     * 
     * @param callback the callback to be run when applying an edit script
     */
    inline void set_stream_callback(stream_callback_f callback) { m_stream_callback = callback; }
    /**
     * @brief check if a stream callback has been set
     * 
     * @return true if the callback has been set
     * @return false otherwise
     */
    inline bool has_stream_callback() const { return m_stream_callback.has_value(); }

    /**
     * @brief return the callback of the stream
     * 
     */
    void stream_callback(const edit_op&) const;

private:

    void check_index_read(size_t index) const;
    void check_index_write(size_t index) const;
    void check_empty() const;

private:

    std::vector<rstream_elem> elems;

    std::optional<stream_callback_f> m_stream_callback;

};

}

#endif // __PDIF_STREAM_HPP__