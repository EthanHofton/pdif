#ifndef __PDIF_LCS_STREAM_DIFFER_HPP__
#define __PDIF_LCS_STREAM_DIFFER_HPP__

#include <pdif/stream_differ_base.hpp>

namespace pdif {

class lcs_stream_differ : public stream_differ_base {
public:

    lcs_stream_differ(const pdif::stream& stream1, const pdif::stream& stream2) : stream_differ_base(stream1, stream2) {}
    ~lcs_stream_differ() override = default;

    /**
     * @brief Implement the diff method to compare the two streams using the Longest Common Subsequence algorithm.
     * 
     * @param diff The diff object to populate with the differences between the two streams.
     */
    virtual void diff(pdif::diff&) override;
};

}

#endif // __PDIF_LCS_STREAM_DIFFER_HPP__