#ifndef __PDIF_DIFF_HPP__
#define __PDIF_DIFF_HPP__

#include <pdif/errors.hpp>
#include <pdif/stream_elem.hpp>
#include <pdif/edit_op.hpp>
#include <pdif/meta_edit_op.hpp>
#include <pdif/logger.hpp>

#include <util/colormod.hpp>

#include <sstream>
#include <optional>

namespace pdif {

/**
 * @brief the class that represents a diff between two streams (edit script)
 * 
 */
class diff {
public:

    /**
     * @brief Represent a chunk of change in the diff
     * 
     */
    struct edit_chunk {
        /**
         * @brief The content of the chunk
         * 
         */
        std::vector<std::string> lines;
        /**
         * @brief The chunk start relative to the original stream
         * 
         */
        int from_file_start;
        /**
         * @brief the chunk start relative to the new stream
         * 
         */
        int to_file_start;
        /**
         * @brief How many lines are in the chunk from the original stream
         * 
         */
        int from_count = 0;
        /**
         * @brief How many lines are in the chunk from the new stream
         * 
         */
        int to_count = 0;
    };

    diff(bool write_console_colors = true) : m_write_console_colors(write_console_colors) {}

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
    void output_edit_script(std::ostream& os) const;

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
     * @brief Set the allowed context line count for edit chunks
     * 
     * @param context 
     */
    inline void set_allowed_context(int context) { m_allowed_context = context; }

    /**
     * @brief Add an original stream to the diff (used for display purposes only)
     * 
     * @param s the original stream
     */
    void add_original_stream(const stream& s) { m_original_streams.push_back(s); }

    std::vector<edit_op>::iterator end() { return m_edit_script.end(); }
    std::vector<edit_op>::iterator begin() { return m_edit_script.begin(); }

    std::vector<edit_chunk> edit_chunk_summary() const;
    void count_op_types(int& plus, int& minus, int&eq) const;

private:


    std::string get_original_line(int index) const;

    friend std::ostream& operator<<(std::ostream& os, const edit_chunk& chunk);

    void check_edit_index(size_t index) const;
    void check_meta_index(size_t index) const;

    void output_summary(std::ostream& os, int plus, int minus, int last, std::string last_char, util::CONSOLE_COLOR_CODE last_color) const;

    void write_edit_chunk(std::ostream& os, const edit_chunk& chunk) const;

    std::string cc(util::CONSOLE_COLOR_CODE code) const;

private:

    std::vector<edit_op> m_edit_script;
    std::vector<meta_edit_op> m_meta_edit_script;
    
    std::vector<stream> m_original_streams;

    bool m_write_console_colors = true;
    int m_allowed_context = 3;
};

std::ostream& operator<<(std::ostream& os, const diff::edit_chunk& chunk);

}

#endif // __PDIF_DIFF_HPP__