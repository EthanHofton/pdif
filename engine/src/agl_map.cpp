#include <pdif/agl_map.hpp>

namespace pdif {

std::map<std::string, std::string> pdif::agl_map::m_agl_map;
std::set<std::string> pdif::agl_map::m_warnings;

std::string agl_map::normalizeUTF8(std::string hexInput, int add) {
    std::string decoded;
    for (int i = 0; (size_t)i < hexInput.size(); i+=4) {
        int unicodeInt = std::stoi(hexInput.substr(i, 4), nullptr, 16);
        unicodeInt += add;
        char utf8char[4]; // UTF-8 character buffer
        int utf8len = utf8proc_encode_char(static_cast<utf8proc_int32_t>(unicodeInt), (uint8_t*)utf8char);
        // error handle
        if (utf8len < 0) {
            PDIF_LOG_ERROR("Failed to encode unicode character {} ({}) in normalizeUTF8", unicodeInt, hexInput.substr(i, 4));
            throw std::runtime_error("Failed to encode unicode character");
        }
        utf8char[utf8len] = '\0'; // Null-terminate the UTF-8 string
        // Normalize the UTF-8 string
        utf8proc_uint8_t *normalizedUtf8 = utf8proc_NFKD((utf8proc_uint8_t*)utf8char);
        // error handle
        if (normalizedUtf8 == nullptr) {
            PDIF_LOG_ERROR("Failed to normalize unicode character {} ({}) in normalizeUTF8", unicodeInt, hexInput.substr(i, 4));
            throw std::runtime_error("Failed to normalize unicode character");
        }
        // Convert the normalized UTF-8 string to a C++ string
        std::string normalizedString((char*)normalizedUtf8);
        decoded.append(normalizedString);
    }
    return decoded;
}

std::string agl_map::glyph_to_utf8(const std::string& glyph) {
    if (m_agl_map.empty()) {
        load_agl_map("/agl_map.txt");
        load_agl_map("/agl_latex_extention.txt");
    }

    if (m_agl_map.find(glyph) != m_agl_map.end()) {
        return m_agl_map[glyph];
    }

    if (m_warnings.find(glyph) == m_warnings.end()) {
        m_warnings.insert(glyph);
        PDIF_LOG_WARN("Glyph {} not found in AGL map", glyph);
    }

    return glyph;
}

void agl_map::load_agl_map(const std::string& file_path) {
    std::ifstream file(std::string(PDIF_RES_PATH) + file_path);

    if (!file.is_open()) {
        PDIF_LOG_ERROR("Failed to open AGL map file");
        throw std::runtime_error("Failed to open AGL map file");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == '#') {
            continue;
        }

        size_t pos = line.find(";");

        if (pos == std::string::npos) {
            PDIF_LOG_ERROR("Invalid line in AGL map file: {}", line);
            throw std::runtime_error("Invalid line in AGL map file");
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        std::string utf8_value = normalizeUTF8(value);

        m_agl_map[key] = utf8_value;
    }

    file.close();
}

} // namespace pdif