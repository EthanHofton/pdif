#include <pdif/diff.hpp>

namespace pdif {

void diff::add_edit_op(const edit_op& op) {
    m_edit_script.push_back(op);
}

void diff::check_edit_index(size_t index) const {
    if (index >= m_edit_script.size()) {
        PDIF_LOG_ERROR("pdif::diff - index out of range");
        throw pdif_out_of_bounds("pdif::diff - index out of range");
    }
}

void diff::check_meta_index(size_t index) const {
    if (index >= m_meta_edit_script.size()) {
        PDIF_LOG_ERROR("pdif::diff - index out of range");
        throw pdif_out_of_bounds("pdif::diff - index out of range");
    }
}

edit_op diff::get_edit_op(size_t index) const {
    check_edit_index(index);
    return m_edit_script[index];
}

size_t diff::edit_op_size() const {
    return m_edit_script.size();
}

void diff::add_meta_edit_op(const meta_edit_op& op) {
    m_meta_edit_script.push_back(op);
}

meta_edit_op diff::get_meta_edit_op(size_t index) const {
    check_meta_index(index);
    return m_meta_edit_script[index];
}

size_t diff::meta_edit_op_size() const {
    return m_meta_edit_script.size();
}

std::string diff::to_json() const {
    throw pdif::pdif_not_implemented("pdif::diff::to_json - not implemented");
} 

void diff::from_json(const std::string&) {
    throw pdif::pdif_not_implemented("pdif::diff::from_json - not implemented");
}

void diff::apply_edit_script(stream& stream) const {
    size_t index = 0;
    for (const edit_op& op : m_edit_script) {
        op.execute(stream, index);
        
        if (stream.has_stream_callback()) {
            try {
                stream.stream_callback(op);
            } catch (const pdif::pdif_invalid_operation& e) {
                PDIF_LOG_ERROR("pdif::diff::apply_edit_script - invalid operation");
                throw e;
            } catch (const pdif::pdif_error_in_callback& e) {
                PDIF_LOG_ERROR("pdif::diff::apply_edit_script - error in callback");
                throw e;
            }
        }
    }
}

void diff::apply_meta_edit_script(stream& stream) const {
    for (const meta_edit_op& op : m_meta_edit_script) {
        op.execute(stream);
        
        if (stream.has_meta_callback()) {
            try {
                stream.meta_callback(op);
            } catch (const pdif::pdif_invalid_operation& e) {
                PDIF_LOG_ERROR("pdif::diff::apply_meta_edit_script - invalid operation");
                throw e;
            } catch (const pdif::pdif_error_in_callback& e) {
                PDIF_LOG_ERROR("pdif::diff::apply_meta_edit_script - error in callback");
                throw e;
            }
        }
    }
}

}