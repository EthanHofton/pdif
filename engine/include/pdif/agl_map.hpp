#ifndef __PDIF_AGL_MAP_HPP__
#define __PDIF_AGL_MAP_HPP__

#include <pdif/logger.hpp>
#include <pdif/pdif_engine_config.hpp>

#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <string>

#include <utf8proc.h>

namespace pdif {

class agl_map {
public:

    /**
     * @brief Convert an adobe glyph name to a utf8 string
     * 
     * @param glyph the glyph name
     * @return std::string 
     */
    static std::string glyph_to_utf8(const std::string& glyph);
    /**
     * @brief do unicode normalization on a hex string input encoded in unicode
     * 
     * @param hexInput hex string, 2 bytes per character
     * @param add add this value to the unicode codepoint
     * @return std::string the normalized utf8 string
     */
    static std::string normalizeUTF8(std::string hexInput, int add = 0);

private:

    /**
     * @brief The map of glyph names to utf8 strings
     * 
     */
    static std::map<std::string, std::string> m_agl_map;
    static std::set<std::string> m_warnings;

    /**
     * @brief load the adobe glyph list map
     * 
     * @param file_path the file
     */
    static void load_agl_map(const std::string& file_path);
};

} // namespace pdif

#endif // __PDIF_AGL_MAP_HPP__