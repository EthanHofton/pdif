#ifndef __PDIF_DIFFER_HPP__
#define __PDIF_DIFFER_HPP__

#include <functional>
#include <string>

namespace pdif {

extern void lcs(const std::string& a, const std::string& b);
extern void myers(const std::string& a, const std::string& b);

}

#endif // __PDIF_DIFFER_HPP__