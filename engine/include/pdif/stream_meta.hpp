#ifndef __PDIF_STREAM_META_HPP__
#define __PDIF_STREAM_META_HPP__

#include <optional>
#include <map>
#include <functional>
#include <pdif/errors.hpp>
#include <pdif/logger.hpp>

namespace pdif {

// meta_edit_op forward declare
class meta_edit_op;

class stream_meta {
public:
    /**
     * @brief the callback function to be used when executing a meta edit operation
     * 
     */
    using meta_callback_f = std::function<void(const meta_edit_op&)>;

    /**
     * @brief Construct a new stream meta object
     * 
     */
    stream_meta() = default;

    /**
     * @brief add metadata to the stream
     * 
     * @param t_key the key of the metadata
     * @param t_value the value of the metadata
     */
    void add_metadata(const std::string& t_key, const std::string& t_value);
    /**
     * @brief remove metadata
     * 
     * @param t_key the key to remove
     */
    void remove_metadata(const std::string& t_key);
    /**
     * @brief update metadata
     * 
     * @param t_key the key of the field to update
     * @param t_value the value to update
     */
    void update_metadata(const std::string& t_key, const std::string& t_value);

    /**
     * @brief Get the metadata object
     * 
     * @param t_key the key
     * @return std::string the value
     */
    std::string get_metadata(const std::string& t_key) const;

    /**
     * @brief check if the stream has a key
     * 
     * @param t_key the key to check for
     * @return true if the key exists
     * @return false otherwise
     */
    bool has_key(const std::string& t_key) const;

    /**
     * @brief Get the metadata object
     * 
     * @return std::map<std::string, std::string> 
     */
    inline std::map<std::string, std::string> get_metadata() const { return m_metadata; }

    /**
     * @brief Set the meta callback object
     * 
     * @param callback the callback to be run when applying an edit script
     */
    inline void set_meta_callback(meta_callback_f callback) { m_meta_callback = callback; }
    /**
     * @brief check if a meta callback has been set
     * 
     * @return true if the callback has been set
     * @return false otherwise
     */
    inline bool has_meta_callback() const { return m_meta_callback.has_value(); }
    /**
     * @brief return the meta callback
     * 
     */
    void meta_callback(const meta_edit_op&) const;

private:

    std::map<std::string, std::string> m_metadata;
    std::optional<meta_callback_f> m_meta_callback;
};

}


#endif // __PDIF_STREAM_META_HPP__