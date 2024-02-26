#include <pdif/stream_differ_base.hpp>

namespace pdif {

void stream_differ_base::meta_diff(pdif::diff& d) {
    auto stream1_metadata = stream1_meta.get_metadata();
    auto stream2_metadata = stream2_meta.get_metadata();    

    for (auto& meta : stream1_metadata) {
        if (!stream1_meta.has_key(meta.first)) {
            // meta is in stream 2 but not in stream 1. add op to diff
            d.add_meta_edit_op(meta_edit_op(meta_edit_op_type::META_ADD, meta.first, meta.second));
        }
        
        if (stream1_meta.has_key(meta.first)) {
            // meta is in both streams. compare values
            if (stream1_meta.get_metadata(meta.first) != stream2_meta.get_metadata(meta.first)) {
                // meta is in both streams but values are different. add op to diff
                d.add_meta_edit_op(meta_edit_op(meta_edit_op_type::META_UPDATE, meta.first, meta.second));
            }
        }
    }

    for (auto &meta : stream1_metadata) {
        if (!stream2_meta.has_key(meta.first)) {
            // meta is in stream 1 but not in stream 2. add op to diff
            d.add_meta_edit_op(meta_edit_op(meta_edit_op_type::META_DELETE, meta.first));
        }
    }
}

}