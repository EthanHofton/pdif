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
    throw pdif::pdif_not_implemented("edit_op::execute - not implemented");
}

}