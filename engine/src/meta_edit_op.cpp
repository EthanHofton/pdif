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

void meta_edit_op::execute(pdif::stream_meta& t_stream) const {
    switch (m_type) {
        case meta_edit_op_type::META_ADD:
            try {
                t_stream.add_metadata(m_meta_key, m_meta_val.value());
                PDIF_LOG_INFO("pdif::meta_edit_op - META_ADD succeeded: key={}, val={}", m_meta_key, m_meta_val.value());
            } catch (const pdif::pdif_invalid_key& e) {
                PDIF_LOG_ERROR("pdif::meta_edit_op - META_ADD failed: {}", e.what());
                throw pdif::pdif_invalid_operation("pdif::meta_edit_op - META_ADD failed");
            }
            break;
        case meta_edit_op_type::META_DELETE:
            try {
                t_stream.remove_metadata(m_meta_key);
                PDIF_LOG_INFO("pdif::meta_edit_op - META_DELETE succeeded: key={}", m_meta_key);
            } catch (const pdif::pdif_invalid_key& e) {
                PDIF_LOG_ERROR("pdif::meta_edit_op - META_DELETE failed: {}", e.what());
                throw pdif::pdif_invalid_operation("pdif::meta_edit_op - META_DELETE failed");
            }
            break;
        case meta_edit_op_type::META_UPDATE:
            try {
                t_stream.update_metadata(m_meta_key, m_meta_val.value());
                PDIF_LOG_INFO("pdif::meta_edit_op - META_UPDATE succeeded: key={}, val={}", m_meta_key, m_meta_val.value());
            } catch (const pdif::pdif_invalid_key& e) {
                PDIF_LOG_ERROR("pdif::meta_edit_op - META_UPDATE failed: {}", e.what());
                throw pdif::pdif_invalid_operation("pdif::meta_edit_op - META_UPDATE failed");
            }
            break;
    }
}

}