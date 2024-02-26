#ifndef __PDIF_STREAM_DIFFER_BASE_HPP__
#define __PDIF_STREAM_DIFFER_BASE_HPP__

#include <functional>
#include <string>

#include <pdif/stream.hpp>
#include <pdif/stream_meta.hpp>
#include <pdif/errors.hpp>
#include <pdif/edit_op.hpp>
#include <pdif/diff.hpp>

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
    stream_differ_base(const pdif::stream& stream1, const pdif::stream_meta& stream1_meta, const pdif::stream& stream2, const pdif::stream_meta& stream2_meta) : stream1(stream1), stream1_meta(stream1_meta), stream2(stream2), stream2_meta(stream2_meta) {}

    /**
     * @brief Destroy the stream differ base object
     * 
     */
    virtual ~stream_differ_base() = default;
    

    /**
     * @brief diff the two streams
     * 
     * @return pdif::diff the edit script between the two streams 
     */
    virtual void diff(pdif::diff&) = 0;

    /**
     * @brief do the meta diff between one stream to another
     * 
     * @param d the diff to be modified
     */
    void meta_diff(pdif::diff& d);

private:

    pdif::stream stream1;
    pdif::stream_meta stream1_meta;
    pdif::stream stream2;
    pdif::stream_meta stream2_meta;

};

}

#endif // __PDIF_STREAM_DIFFER_BASE_HPP__