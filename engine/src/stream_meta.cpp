#include <pdif/stream_meta.hpp>

namespace pdif {

void stream_meta::add_metadata(const std::string& t_key, const std::string& t_value) {
    if (has_key(t_key)) {
        PDIF_LOG_ERROR("stream_meta::add_metadata: Key already exists: {}", t_key);
        throw pdif::pdif_invalid_key("stream_meta::add_metadata: Key already exists: " + t_key);
    }

    m_metadata.emplace(t_key, t_value);
}

void stream_meta::remove_metadata(const std::string& t_key) {
    if (!has_key(t_key)) {
        PDIF_LOG_ERROR("stream::remove_metadata: Key does not exist: {}", t_key);
        throw pdif::pdif_invalid_key("stream_meta::remove_metadata: Key does not exist: " + t_key);
    }

    m_metadata.erase(t_key);
}

void stream_meta::update_metadata(const std::string& t_key, const std::string& t_value) {
    if (!has_key(t_key)) {
        PDIF_LOG_ERROR("stream_meta::update_metadata: Key does not exist: {}", t_key);
        throw pdif::pdif_invalid_key("stream_meta::update_metadata: Key does not exist: " + t_key);
    }

    m_metadata[t_key] = t_value;
}

std::string stream_meta::get_metadata(const std::string& t_key) const {
    if (!has_key(t_key)) {
        PDIF_LOG_ERROR("stream_meta::get_metadata: Key does not exist: {}", t_key);
        throw pdif::pdif_invalid_key("stream_meta::get_metadata: Key does not exist: " + t_key);
    }

    return m_metadata.at(t_key);
}

bool stream_meta::has_key(const std::string& t_key) const {
    if (m_metadata.find(t_key) == m_metadata.end()) {
        return false;
    }

    return true;
}

void stream_meta::meta_callback(const pdif::meta_edit_op& t_op) const {
    if (!has_meta_callback()) {
        PDIF_LOG_ERROR("stream_meta::meta_callback: Meta callback not set");
        throw pdif::pdif_invalid_operation("stream_meta::meta_callback: Meta callback not set");
    }

    try {
        m_meta_callback.value()(std::forward<const pdif::meta_edit_op&>(t_op));
    } catch (const std::exception& e) {
        PDIF_LOG_ERROR("stream::meta_callback error in callback: {}", e.what());
        throw pdif::pdif_error_in_callback("stream::meta_callback error in callback: " + std::string(e.what()));
    }
}

} // namespace pdif
