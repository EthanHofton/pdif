#ifndef __PDIF_DIFF_HPP__
#define __PDIF_DIFF_HPP__

#include <pdif/errors.hpp>
#include <pdif/stream_elem.hpp>
#include <pdif/edit_op.hpp>
#include <pdif/meta_edit_op.hpp>
#include <pdif/logger.hpp>

#include <nlohmann/json.hpp>
#include <util/colormod.hpp>

#include <sstream>
#include <optional>

using json = nlohmann::json;

namespace pdif {

/**
 * @brief the class that represents a diff between two streams (edit script)
 * 
 */
class diff {
public:

    /**
     * @brief add an edit op to the edit script
     * 
     * @param op the operation to add
     */
    void add_edit_op(const edit_op& op);
    /**
     * @brief Get the edit op object
     * 
     * @param index the index of the edit op
     * @return edit_op the edit op at that index
     */
    edit_op get_edit_op(size_t index) const;
    /**
     * @brief get the size of the edit op edit script
     * 
     * @return size_t the size
     */
    size_t edit_op_size() const;

    /**
     * @brief add a meta edit op to the meta edit script
     * 
     * @param op the operation to add
     */
    void add_meta_edit_op(const meta_edit_op& op);
    /**
     * @brief Get the meta edit op object
     * 
     * @param index the index of the meta edit op
     * @return meta_edit_op 
     */
    meta_edit_op get_meta_edit_op(size_t index) const;
    /**
     * @brief the size of the meta edit op script
     * 
     * @return size_t 
     */
    size_t meta_edit_op_size() const;

    /**
     * @brief convert the edit script to a json string
     * 
     * @return std::string the json string
     */
    [[deprecated("this function has not been tested. Do not use it")]]
    std::string to_json() const;
    /**
     * @brief read the edit script from a json string
     * 
     * @param json the json string
     */
    [[deprecated("this function has not been tested. Do not use it")]]
    void from_json(const std::string& json);

    /**
     * @brief apply the edit script to the given stream
     * 
     * @param stream the stream to apply the edit script to
     */
    void apply_edit_script(stream& stream) const;

    /**
     * @brief apply the meta edit script to the given stream
     * 
     * @param stream the stream to apply the meta edit script to
     */
    void apply_meta_edit_script(stream_meta& stream) const;

    /**
     * @brief output the edit script to the given output stream
     * 
     * @param os the output stream
     */
    void output_edit_script(std::ostream& os, std::optional<stream> = std::nullopt, bool print_eq = false) const;

    /**
     * @brief output the meta edit script to the given output stream
     * 
     * @param os the output stream
     */
    void output_meta_edit_script(std::ostream& os) const;

    /**
     * @brief reverse the edit script
     * 
     */
    inline void reverse_edit_ops() { std::reverse(m_edit_script.begin(), m_edit_script.end()); }
    /**
     * @brief reverse the edit script from start to end
     * 
     * @param start the start iterator
     * @param end the end iterator
     */
    inline void reverse_edit_ops(std::vector<edit_op>::iterator start, std::vector<edit_op>::iterator end) { std::reverse(start, end); }

    /**
     * @brief Add an original stream to the diff (used for display purposes only)
     * 
     * @param s the original stream
     */
    void add_original_stream(const stream& s) { m_original_streams.push_back(s); }

    std::vector<edit_op>::iterator end() { return m_edit_script.end(); }
    std::vector<edit_op>::iterator begin() { return m_edit_script.begin(); }

private:

    struct edit_chunk {
        std::vector<std::string> lines;
        int from_file_start;
        int to_file_start;
        int from_count = 0;
        int to_count = 0;

        static const int ALLOWED_CONTEXT = 3;
    };

    std::string get_original_line(int index) const;

    friend std::ostream& operator<<(std::ostream& os, const edit_chunk& chunk);

    void check_edit_index(size_t index) const;
    void check_meta_index(size_t index) const;
    
    json edit_op_to_json(const edit_op& op) const;
    json meta_edit_op_to_json(const meta_edit_op& op) const;

    edit_op edit_op_from_json(const json& j) const;
    meta_edit_op meta_edit_op_from_json(const json& j) const;

    void output_summary(std::ostream& os, int plus, int minus, int last, std::string last_char, util::CONSOLE_COLOR_CODE last_color) const;

private:

    std::vector<edit_op> m_edit_script;
    std::vector<meta_edit_op> m_meta_edit_script;
    
    std::vector<stream> m_original_streams;
};

std::ostream& operator<<(std::ostream& os, const diff::edit_chunk& chunk);

}

#endif // __PDIF_DIFF_HPP__