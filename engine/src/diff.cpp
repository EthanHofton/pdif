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

void diff::apply_meta_edit_script(stream_meta& stream) const {
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

std::vector<diff::edit_chunk> diff::edit_chunk_summary() const {
    std::vector<edit_chunk> chunks;

    bool in_chunk = false;
    edit_chunk chunk;

    int from_file_pointer = 0;
    int to_file_pointer = 0;
    int context_remaining = m_allowed_context;

    for (size_t i = 0; i < m_edit_script.size(); i++) {
        auto op = m_edit_script[i];

        if (op.get_type() == edit_op_type::EQ) {
            if (in_chunk) {
                if (context_remaining > 0) {
                    chunk.from_count++;
                    chunk.to_count++;
                    chunk.lines.push_back(get_original_line(from_file_pointer));
                    context_remaining--;
                } else {
                    chunks.push_back(chunk);
                    chunk = edit_chunk();
                    in_chunk = false;
                }
            }

            from_file_pointer++;
            to_file_pointer++;
        } else {
            context_remaining = m_allowed_context;
            if (!in_chunk) {
                int pre_context_lines = 0;                    

                // add ALLOWED_CONTEXT lines of context
                for (int j = m_allowed_context; j > 0; j--) {
                    if (from_file_pointer - j < 0) {
                        continue;
                    }

                    pre_context_lines++;
                    chunk.from_count++;
                    chunk.to_count++;
                    chunk.lines.push_back(get_original_line(from_file_pointer - j));
                }

                in_chunk = true;
                chunk.from_file_start = from_file_pointer - pre_context_lines;
                chunk.to_file_start = to_file_pointer - pre_context_lines;

            }

            if (op.get_type() == edit_op_type::INSERT) {
                chunk.to_count++;

                std::stringstream new_line_ss;
                new_line_ss << cc(util::CONSOLE_COLOR_CODE::FG_GREEN) << "+" << op.get_arg()->to_string(m_write_console_colors) << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT);
                chunk.lines.push_back(new_line_ss.str());
            } else if (op.get_type() == edit_op_type::DELETE) {
                chunk.from_count++;

                std::stringstream new_line_ss;
                new_line_ss << cc(util::CONSOLE_COLOR_CODE::FG_RED) << "-" << get_original_line(from_file_pointer) << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT);
                chunk.lines.push_back(new_line_ss.str());
            }

            // update the pointers
            if (op.get_type() == edit_op_type::DELETE) {
                from_file_pointer++;
            } else if (op.get_type() == edit_op_type::INSERT) {
                to_file_pointer++;
            }
        }
    }

    if (in_chunk) {
        chunks.push_back(chunk);
    }

    return chunks;
}

void diff::count_op_types(int& plus, int& minus, int&eq) const {
    plus = 0;
    minus = 0;
    eq = 0;

    for (const edit_op& op : m_edit_script) {
        if (op.get_type() == edit_op_type::INSERT) {
            plus++;
        } else if (op.get_type() == edit_op_type::DELETE) {
            minus++;
        } else if (op.get_type() == edit_op_type::EQ) {
            eq++;
        }
    }
}

void diff::output_edit_script(std::ostream& os) const {
    os << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "Content Differences" << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;
    os << std::endl;


    if (m_edit_script.size() == 0) {
        os << "No differences" << std::endl;
    } else {
        auto chunks = edit_chunk_summary();
        for (const edit_chunk& chunk : chunks) {
            write_edit_chunk(os, chunk);
        }
    }
    
    int plus_count, minus_count, eq_count;
    count_op_types(plus_count, minus_count, eq_count);

    output_summary(os, plus_count, minus_count, eq_count, "=", util::CONSOLE_COLOR_CODE::FG_DEFAULT);
    os << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "End of Content Differences" << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;
}

std::string diff::get_original_line(int index) const {
    int i = index;
    for (size_t y = 0; y < m_original_streams.size(); y++) {
        if (m_original_streams[y].size() <= (size_t)i) {
            i -= m_original_streams[y].size();
            if (i < 0) {
                throw pdif_out_of_bounds("pdif::diff::get_original_line - index out of range");
            }
        } else {
            return m_original_streams[y][i]->to_string(m_write_console_colors);
        }
    }

    throw pdif_out_of_bounds("pdif::diff::get_original_line - index out of range");
}

void diff::output_meta_edit_script(std::ostream& os) const {
    os << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "Meta Differences" << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;
    os << std::endl;

    int plus = 0;
    int minus = 0;
    int update = 0;
    
    if (m_meta_edit_script.size() == 0) {
        os << "\tNo differences" << std::endl;
    } else {
        for (const meta_edit_op& op : m_meta_edit_script) {
            switch (op.get_type()) {
                case meta_edit_op_type::META_ADD:
                    os << "\t" << cc(util::CONSOLE_COLOR_CODE::FG_GREEN) << "+ ";
                    os << op.get_meta_key();
                    os << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT) << " ==> " << cc(util::CONSOLE_COLOR_CODE::FG_GREEN);
                    os << op.get_meta_val();
                    os << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT);
                    os << std::endl;
                    plus++;
                    break;
                case meta_edit_op_type::META_DELETE:
                    os << "\t" << cc(util::CONSOLE_COLOR_CODE::FG_RED) << "- ";
                    os << op.get_meta_key();
                    os << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT);
                    os << std::endl;
                    minus++;
                    break;
                case meta_edit_op_type::META_UPDATE:
                    os << "\t" << cc(util::CONSOLE_COLOR_CODE::FG_YELLOW) << "~ ";
                    os << op.get_meta_key();
                    os << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT) << " ==> " << cc(util::CONSOLE_COLOR_CODE::FG_YELLOW);
                    os << op.get_meta_val();
                    os << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT);
                    os << std::endl;
                    update++;
                    break;
                default:
                    break;
            }
        }
    }

    output_summary(os, plus, minus, update, "~", util::CONSOLE_COLOR_CODE::FG_YELLOW);
    os << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "End of Meta Differences" << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET) << std::endl;

}

void diff::output_summary(std::ostream& os, int plus, int minus, int last, std::string last_char, util::CONSOLE_COLOR_CODE last_color) const {
    os << std::endl;
    os << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD) << "Summary: " << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET);
    os << cc(util::CONSOLE_COLOR_CODE::FG_GREEN) << "+" << plus << " ";
    os << cc(util::CONSOLE_COLOR_CODE::FG_RED) << "-" << minus << " ";
    os << cc(last_color) << last_char << last;
    os << cc(util::CONSOLE_COLOR_CODE::FG_DEFAULT);
    os << std::endl;
}

std::string diff::cc(util::CONSOLE_COLOR_CODE code) const {
    if (m_write_console_colors) {
        std::stringstream ss;
        ss << code;
        return ss.str();
    }
    return "";
}

void diff::write_edit_chunk(std::ostream& os, const edit_chunk& chunk) const {
    // print chunk header
    os << cc(util::CONSOLE_COLOR_CODE::TEXT_BOLD);
    os << "@@ -" << chunk.from_file_start << "," << chunk.from_count << " +" << chunk.to_file_start << "," << chunk.to_count << " @@" << std::endl;
    os << cc(util::CONSOLE_COLOR_CODE::TEXT_RESET);
    
    // print chunk content
    for (const std::string& line : chunk.lines) {
        os << line << std::endl;
    }

}

std::ostream& operator<<(std::ostream& os, const diff::edit_chunk& chunk) {
    // print chunk header
    os << util::CONSOLE_COLOR_CODE::TEXT_BOLD;
    os << "@@ -" << chunk.from_file_start << "," << chunk.from_count << " +" << chunk.to_file_start << "," << chunk.to_count << " @@" << std::endl;
    os << util::CONSOLE_COLOR_CODE::TEXT_RESET;
    
    // print chunk content
    for (const std::string& line : chunk.lines) {
        os << line << std::endl;
    }

    return os;
}


} // namespace pdif