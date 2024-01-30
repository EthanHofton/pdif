#ifndef __PDIF_STREAM_DIFFER_BASE_HPP__
#define __PDIF_STREAM_DIFFER_BASE_HPP__

#include <functional>
#include <string>

#include <pdif/stream.hpp>
#include <pdif/errors.hpp>

namespace pdif {

/**
 * @brief a base class for stream differ
 * 
 * To be implemented using a diffing algorithm such as LCS or Myers.
 * 
 */
class stream_differ_base {
public:

    /**
     * @brief Construct a new stream differ base object
     * 
     * @param stream1 the first stream
     * @param stream2 the second stream
     */
    stream_differ_base(const pdif::stream& stream1, const pdif::stream& stream2) : stream1(stream1), stream2(stream2) {}

    /**
     * @brief Destroy the stream differ base object
     * 
     */
    virtual ~stream_differ_base() = default;
    

    virtual void diff() = 0;

    void meta_diff();

private:

    pdif::stream stream1;
    pdif::stream stream2;

};

}

#endif // __PDIF_STREAM_DIFFER_BASE_HPP__