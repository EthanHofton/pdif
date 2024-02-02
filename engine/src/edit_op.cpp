#include <pdif/edit_op.hpp>

namespace pdif {

edit_op::edit_op(edit_op_type type, std::optional<pdif::rstream_elem> t_arg) : type(type), arg(t_arg) {
    if (type != edit_op_type::INSERT && has_arg()) {
        PDIF_LOG_ERROR("edit_op::edit_op - arg is only valid for INSERT edit_op");
        throw pdif::pdif_invalid_argment("edit_op::edit_op - arg is only valid for INSERT edit_op");
    }

    if (type == edit_op_type::INSERT && !has_arg()) {
        PDIF_LOG_ERROR("edit_op::edit_op - arg is required for INSERT edit_op");
        throw pdif::pdif_invalid_argment("edit_op::edit_op - arg is required for INSERT edit_op");
    }
}

void edit_op::execute(pdif::stream& t_stream, size_t& t_index) const {
    // switch on the edit_op type
    switch (type) {
        case edit_op_type::INSERT: {
            // try push to the stream
            try {
                PDIF_LOG_INFO("edit_op::execute - (INSERT) inserting element into stream at index {}", t_index);
                t_stream.push(t_index, arg.value());
                ++t_index;
            } catch (const pdif::pdif_out_of_bounds& e) {
                PDIF_LOG_ERROR("edit_op::execute - failed to insert element into stream. Reason: {}", e.what());
                // rethrow
                throw pdif::pdif_out_of_bounds("edit_op::execute - failed to insert element into stream");
            }
            break;
        }
        case edit_op_type::DELETE: {
            // try to delete from the stream
            try {
                PDIF_LOG_INFO("edit_op::execute - (DELETE) deleting element from stream at index {}", t_index);
                t_stream.pop(t_index);
            } catch (const pdif::pdif_out_of_bounds& e) {
                PDIF_LOG_ERROR("edit_op::execute - failed to delete element from stream. Reason: {}", e.what());
                // rethrow
                throw pdif::pdif_out_of_bounds("edit_op::execute - failed to delete element from stream");
            }
            break;
        }
        case edit_op_type::EQ: {
            PDIF_LOG_INFO("edit_op::execute - (EQ) skipping element in stream at index {}", t_index);
            // increment the index
            ++t_index;
            break;
        }
    }
}

}