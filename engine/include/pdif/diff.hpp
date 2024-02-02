#ifndef __PDIF_DIFF_HPP__
#define __PDIF_DIFF_HPP__

#include <pdif/errors.hpp>
#include <pdif/stream_elem.hpp>
#include <pdif/edit_op.hpp>
#include <pdif/logger.hpp>
#include <sstream>

namespace pdif {

/**
 * @brief the class that represents a diff between two streams (edit script)
 * 
 */
class diff {
public:

    /**
     * @brief add an edit operation to the edit script
     * 
     * @param op 
     */
    void add_edit_op(const edit_op& op);
    
    /**
     * @brief get the edit operation at the given index
     * 
     * @param index the index of the edit operation
     * @return edit_op the edit operation at the given index
     */
    edit_op operator[](size_t index) const;
    
    /**
     * @brief the size of the edit script
     * 
     * @return size_t the size of the edit script
     */
    size_t size() const;

    /**
     * @brief convert the edit script to a json string
     * 
     * @return std::string the json string
     */
    std::string to_json() const;
    /**
     * @brief read the edit script from a json string
     * 
     * @param json the json string
     */
    void from_json(const std::string& json);

    /**
     * @brief apply the edit script to the given stream
     * 
     * @param stream the stream to apply the edit script to
     */
    void apply(stream& stream) const;

private:

    void check_index(size_t index) const;

private:

    std::vector<edit_op> m_edit_script;
};

}

#endif // __PDIF_DIFF_HPP__