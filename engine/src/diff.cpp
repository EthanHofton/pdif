#include <pdif/diff.hpp>

namespace pdif {

void diff::add_edit_op(const edit_op& op) {
    m_edit_script.push_back(op);
}

void diff::check_index(size_t index) const {
    if (index >= m_edit_script.size()) {
        PDIF_LOG_ERROR("pdif::diff - index out of range");
        throw pdif_out_of_bounds("pdif::diff - index out of range");
    }
}

edit_op diff::operator[](size_t index) const {
    check_index(index);
    return m_edit_script[index];
}

size_t diff::size() const {
    return m_edit_script.size();
}

std::string diff::to_json() const {
    std::string spaces = "";
    std::string tab = "    ";
    std::stringstream json;

    json << spaces << "{" << std::endl;

    spaces += tab;

    json << spaces << "\"edit_script\": [" << std::endl;

    spaces += tab;

    for (size_t i = 0; i < m_edit_script.size(); i++) {
        edit_op op = m_edit_script[i];
        json << spaces << "{";

        spaces += tab;

        json << spaces << "\"type\": ";
        json << "\"" << op.get_type() << "\"," << std::endl;

        if (op.has_arg()) {
            json << spaces << "\"arg\": {";
            spaces += tab;

            json << spaces << "\"type\": ";
            json << "\"" << op.get_arg()->type() << "\"," << std::endl;

            json << spaces << "\"val\": ";
            json << "\"" << op.get_arg()->to_string() << "\"" << std::endl;

            spaces = spaces.substr(0, spaces.size() - tab.size());
            json << spaces << "}" << std::endl;
        }

        spaces = spaces.substr(0, spaces.size() - tab.size());
        json << spaces << "}";

        if (i != m_edit_script.size() - 1) {
            json << ",";
        }

        json << std::endl;
    }

    spaces = spaces.substr(0, spaces.size() - tab.size());
    json << spaces << "]" << std::endl;

    spaces = spaces.substr(0, spaces.size() - tab.size());
    json << spaces << "}";

    return json.str();
} 

void diff::from_json(const std::string&) {
    throw pdif::pdif_not_implemented("pdif::diff::from_json - not implemented");
}

void diff::apply(stream& stream) const {
    size_t index = 0;
    for (const edit_op& op : m_edit_script) {
        op.execute(stream, index);

        // TODO: Add a callback for each edit operation
    }
}

}