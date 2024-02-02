#ifndef __PDIF_EDIT_OP_HPP__
#define __PDIF_EDIT_OP_HPP__

#include <pdif/errors.hpp>
#include <pdif/stream_elem.hpp>
#include <pdif/stream.hpp>
#include <pdif/logger.hpp>
#include <util/memory.hpp>
#include <optional>

namespace pdif {

/**
 * @brief enum type for edit operations
 * 
 */
enum class edit_op_type {
    DELETE = 0,
    EQ = 1,
    INSERT = 2,
};

/**
 * @brief << operator for edit_op_type
 * 
 * @param os the ostream to write to
 * @param type the edit_op_type to write
 * @return std::ostream& the ostream
 */
inline std::ostream& operator<<(std::ostream& os, const edit_op_type& type) {
    switch (type) {
        case edit_op_type::INSERT:
            os << "INSERT";
            break;
        case edit_op_type::DELETE:
            os << "DELETE";
            break;
        case edit_op_type::EQ:
            os << "EQ";
            break;
    }
    return os;
}

/**
 * @brief a class to represent a singular edit operation
 * 
 */
class edit_op {
public:

    /**
     * @brief Construct a new edit op object
     * 
     * @param type the type of edit operaiton
     * @param t_arg the argument for the edit operation. Only valid for INSERT edit_op
     */
    edit_op(edit_op_type type, std::optional<pdif::rstream_elem> t_arg);
    /**
     * @brief Construct a new edit op object
     * 
     * @param type the type of the edit operation
     */
    edit_op(edit_op_type type) : edit_op(type, std::nullopt) {}

    /**
     * @brief Get the type object
     * 
     * @return edit_op_type the type of edit operation
     */
    inline edit_op_type get_type() const {
        return type;
    }

    /**
     * @brief Get the arg object
     * 
     * @return std::optional<pdif::rstream_elem> the argument for the edit operation
     */
    inline pdif::rstream_elem get_arg() const {
        if (!has_arg()) {
            PDIF_LOG_ERROR("edit_op::get_arg - arg is not valid for this edit_op");
            throw pdif::pdif_invalid_argment("edit_op::get_arg - arg is not valid for this edit_op");
        }

        return arg.value();
    }

    /**
     * @brief checks if the edit operation has an argument
     * 
     * @return true if the edit operation has an argument
     * @return false otherwise
     */
    inline bool has_arg() const {
        return arg.has_value();
    }

    /**
     * @brief This function will executer the edit operation on the stream at the given index, and update the index accordingly
     * 
     * @param t_stream the stream to execute the edit operation on
     * @param t_index the index to execute the edit operation at
     */
    void execute(pdif::stream& t_stream, size_t &t_index) const;

private:

    edit_op_type type;
    std::optional<pdif::rstream_elem> arg;

};

}


#endif