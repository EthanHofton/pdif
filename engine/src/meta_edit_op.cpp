#include <pdif/meta_edit_op.hpp>

namespace pdif {

meta_edit_op::meta_edit_op(meta_edit_op_type type, const std::string& meta_key, std::optional<std::string> meta_val) : m_type(type), m_meta_key(meta_key), m_meta_val(meta_val) {
    switch (m_type) {
        case meta_edit_op_type::META_ADD:
        case meta_edit_op_type::META_UPDATE:
            if (!has_meta_val()) {
                PDIF_LOG_ERROR("pdif::meta_edit_op - META_ADD or META_UPDATE must have meta_val arg");
                throw pdif::pdif_invalid_argment("pdif::meta_edit_op - META_ADD or META_UPDATE must have meta_val arg");
            }
            break;
        case meta_edit_op_type::META_DELETE:
            if (has_meta_val()) {
                PDIF_LOG_ERROR("pdif::meta_edit_op - META_DELETE must not have meta_val arg");
                throw pdif::pdif_invalid_argment("pdif::meta_edit_op - META_ADD or META_UPDATE must have meta_val arg");
            }
            break;
    }
}

void meta_edit_op::execute(pdif::stream&) const {
    throw pdif::pdif_not_implemented("meta_edit_op::execute() - not implemented");
}

}