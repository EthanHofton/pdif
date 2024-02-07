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
    json j;

    j["diff"] = {};

    j["diff"]["edit_script"] = json::array();

    for (const edit_op& op : m_edit_script) {
        j["diff"]["edit_script"].push_back(edit_op_to_json(op));
    }

    j["diff"]["meta_edit_script"] = json::array();

    for (const meta_edit_op& op : m_meta_edit_script) {
        j["diff"]["meta_edit_script"].push_back(meta_edit_op_to_json(op));
    }

    return j.dump();
} 

void diff::from_json(const std::string& json_str) {
    json in_json = json::parse(json_str);

    if (!in_json.contains("diff")) {
        PDIF_LOG_ERROR("pdif::diff::from_json - invalid json");
        throw pdif::pdif_invalid_json("pdif::diff::from_json - invalid json");
    }

    // parse edit_script first
    if (!in_json["diff"].contains("edit_script")) {
        PDIF_LOG_ERROR("pdif::diff::from_json - invalid json. No edit script");
        throw pdif::pdif_invalid_json("pdif::diff::from_json - invalid json. No edit script");
    }

    json edit_script = in_json["diff"]["edit_script"];

    if (!edit_script.is_array()) {
        PDIF_LOG_ERROR("pdif::diff::from_json - invalid json. Edit script is not an array");
        throw pdif::pdif_invalid_json("pdif::diff::from_json - invalid json. Edit script is not an array");
    }

    for (const json& op : edit_script) {
        m_edit_script.push_back(edit_op_from_json(op));
    }

    // parse meta_edit_script second
    if (!in_json["diff"].contains("meta_edit_script")) {
        PDIF_LOG_ERROR("pdif::diff::from_json - invalid json. No meta edit script");
        throw pdif::pdif_invalid_json("pdif::diff::from_json - invalid json. No meta edit script");
    }

    json meta_edit_script = in_json["diff"]["meta_edit_script"];

    if (!meta_edit_script.is_array()) {
        PDIF_LOG_ERROR("pdif::diff::from_json - invalid json. Meta edit script is not an array");
        throw pdif::pdif_invalid_json("pdif::diff::from_json - invalid json. Meta edit script is not an array");
    }

    for (const json& op : meta_edit_script) {
        m_meta_edit_script.push_back(meta_edit_op_from_json(op));
    }
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

json diff::edit_op_to_json(const edit_op& op) const {
    json op_j = json::object();

    // get the string type of the operation
    std::stringstream op_type_ss;
    op_type_ss << op.get_type();

    op_j["type"] = op_type_ss.str();

    if (op.has_arg()) {
        op_j["arg"] = json::object();
        op_j["arg"]["val"] = op.get_arg()->to_string();

        std::stringstream arg_type_ss;
        arg_type_ss << op.get_arg()->type();
        op_j["arg"]["type"] = arg_type_ss.str();
    }

    return op_j;
}

json diff::meta_edit_op_to_json(const meta_edit_op& op) const {
    json op_j = json::object();

    std::stringstream op_type_ss;
    op_type_ss << op.get_type();

    op_j["type"] = op_type_ss.str();
    op_j["meta_key"] = op.get_meta_key();

    if (op.has_meta_val()) {
        op_j["meta_val"] = op.get_meta_val();
    }

    return op_j;
}

edit_op diff::edit_op_from_json(const json& j) const {
    if (!j.contains("type")) {
        PDIF_LOG_ERROR("pdif::diff::edit_op_from_json - invalid json");
        throw pdif::pdif_invalid_json("pdif::diff::edit_op_from_json - invalid json");
    }

    std::string string_type = j["type"].get<std::string>();
    std::istringstream ss(string_type);
    edit_op_type t;

    try {
        ss >> t;
    } catch (const pdif::pdif_invalid_argment& e) {
        PDIF_LOG_ERROR("pdif::diff::edit_op_from_json - invalid json");
        throw pdif::pdif_invalid_json("pdif::diff::edit_op_from_json - invalid json");
    }

    if (j.contains("arg")) {
        if (!j["arg"].contains("val") || !j["arg"].contains("type")) {
            PDIF_LOG_ERROR("pdif::diff::edit_op_from_json - invalid json");
            throw pdif::pdif_invalid_json("pdif::diff::edit_op_from_json - invalid json");
        }

        std::string arg_type_string = j["arg"]["type"].get<std::string>();
        std::istringstream arg_ss(arg_type_string);
        pdif::stream_type arg_t;

        try {
            arg_ss >> arg_t;
        } catch (const pdif::pdif_invalid_argment& e) {
            PDIF_LOG_ERROR("pdif::diff::edit_op_from_json - invalid json");
            throw pdif::pdif_invalid_json("pdif::diff::edit_op_from_json - invalid json");
        }

        pdif::rstream_elem arg;
        switch (arg_t) {
            case pdif::stream_type::text:
                arg = pdif::stream_elem::create<pdif::text_elem>(j["arg"]["val"].get<std::string>());
                break;
            case pdif::stream_type::binary:
                throw pdif::pdif_not_implemented("pdif::diff::edit_op_from_json - binary stream not implemented");
                break;
        }

        try {
            return edit_op(t, arg);
        } catch (const pdif::pdif_invalid_argment& e) {
            PDIF_LOG_ERROR("pdif::diff::edit_op_from_json - invalid json");
            throw pdif::pdif_invalid_json("pdif::diff::edit_op_from_json - invalid json");
        }
    } else {
        try {
            return edit_op(t);
        } catch (const pdif::pdif_invalid_argment& e) {
            PDIF_LOG_ERROR("pdif::diff::edit_op_from_json - invalid json");
            throw pdif::pdif_invalid_json("pdif::diff::edit_op_from_json - invalid json");
        }
    }
}

meta_edit_op diff::meta_edit_op_from_json(const json& j) const {

    if (!j.contains("type") || !j.contains("meta_key")) {
        PDIF_LOG_ERROR("pdif::diff::meta_edit_op_from_json - invalid json");
        throw pdif::pdif_invalid_json("pdif::diff::meta_edit_op_from_json - invalid json");
    }

    std::string meta_key = j["meta_key"].get<std::string>();
    std::string string_type = j["type"].get<std::string>();
    std::istringstream ss(string_type);
    meta_edit_op_type t;

    try {
        ss >> t;
    } catch (const pdif::pdif_invalid_argment& e) {
        PDIF_LOG_ERROR("pdif::diff::meta_edit_op_from_json - invalid json");
        throw pdif::pdif_invalid_json("pdif::diff::meta_edit_op_from_json - invalid json");
    }

    if (j.contains("meta_val")) {
        std::string meta_val = j["meta_val"].get<std::string>();
        try {
            return meta_edit_op(t, meta_key, meta_val);
        } catch (const pdif::pdif_invalid_argment& e) {
            PDIF_LOG_ERROR("pdif::diff::meta_edit_op_from_json - invalid json");
            throw pdif::pdif_invalid_json("pdif::diff::meta_edit_op_from_json - invalid json");
        }
    } else {
        try {
            return meta_edit_op(t, meta_key);
        } catch (const pdif::pdif_invalid_argment& e) {
            PDIF_LOG_ERROR("pdif::diff::meta_edit_op_from_json - invalid json");
            throw pdif::pdif_invalid_json("pdif::diff::meta_edit_op_from_json - invalid json");
        }
    }
}

}