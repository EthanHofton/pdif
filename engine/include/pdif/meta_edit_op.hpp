#ifndef __PDIF_META_EDIT_OP__
#define __PDIF_META_EDIT_OP__

#include <pdif/errors.hpp>
#include <pdif/logger.hpp>
#include <pdif/stream.hpp>
#include <optional>

namespace pdif {

/**
 * @brief an enum class used to represent the different type of meta edit operations
 * 
 */
enum class meta_edit_op_type {
    META_DELETE = 0,
    META_ADD = 1,
    META_UPDATE = 2,
};
    
/**
 * @brief print the meta_edit_op_type to an output stream
 * 
 * @param os the output stream
 * @param type the meta_edit_op_type
 * @return std::ostream& the output stream
 */
inline std::ostream& operator<<(std::ostream& os, const meta_edit_op_type& type) {
    switch (type) {
        case meta_edit_op_type::META_ADD:
            os << "META_ADD";
            break;
        case meta_edit_op_type::META_DELETE:
            os << "META_DELETE";
            break;
        case meta_edit_op_type::META_UPDATE:
            os << "META_UPDATE";
            break;
    }

    return os;
}

inline std::istream& operator>>(std::istream& is, meta_edit_op_type& type) {
    std::string str;
    is >> str;

    if (str == "META_ADD") {
        type = meta_edit_op_type::META_ADD;
    } else if (str == "META_DELETE") {
        type = meta_edit_op_type::META_DELETE;
    } else if (str == "META_UPDATE") {
        type = meta_edit_op_type::META_UPDATE;
    } else {
        PDIF_LOG_ERROR("meta_edit_op_type::operator>> - invalid meta_edit_op_type");
        throw pdif::pdif_invalid_argment("meta_edit_op_type::operator>> - invalid meta_edit_op_type");
    }

    return is;
}

class meta_edit_op {
public:

    /**
     * @brief Construct a new meta edit op object
     * 
     * @param type the type of meta edit op
     * @param meta_key the key of the metadata field
     * @param meta_val (optional) value of the metadata field. Only valid for update or add
     */
    meta_edit_op(meta_edit_op_type type, const std::string& meta_key, std::optional<std::string> meta_val);

    /**
     * @brief Construct a new meta edit op object
     * 
     * @param type the type of meta edit op
     * @param meta_key the key of the metadata field
     * 
     * this consturctor is only valid for META_DELETE
     */
    meta_edit_op(meta_edit_op_type type, const std::string& meta_key) : meta_edit_op(type, meta_key, std::nullopt) {}

    /**
     * @brief Get the type object
     * 
     * @return meta_edit_op_type 
     */
    inline meta_edit_op_type get_type() const {
        return m_type;
    }

    /**
     * @brief Get the meta key object
     * 
     * @return std::string 
     */
    inline std::string get_meta_key() const {
        return m_meta_key;
    }

    /**
     * @brief Get the meta val object
     * 
     * @return std::string 
     */
    inline std::string get_meta_val() const {
        if (!has_meta_val()) {
            PDIF_LOG_ERROR("meta_edit_op::get_meta_val - meta_val is not valid for this meta_edit_op");
            throw pdif::pdif_invalid_argment("meta_edit_op::get_meta_val - meta_val is not valid for this meta_edit_op");
        }

        return m_meta_val.value();
    }

    /**
     * @brief checks if the meta value exists
     * 
     * @return true if the meta value exists
     * @return false otherwise
     */
    inline bool has_meta_val() const {
        return m_meta_val.has_value();
    }

    /**
     * @brief execute the meta edit operator on the stream
     * 
     * @param t_stream the stream to execute on
     */
    void execute(pdif::stream& t_stream) const;

private:

    meta_edit_op_type m_type;
    std::string m_meta_key;
    std::optional<std::string> m_meta_val;
};


}

#endif // __PDIF_META_EDIT_OP__