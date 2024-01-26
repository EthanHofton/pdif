#ifndef __PDIF_ERRORS_HPP__
#define __PDIF_ERRORS_HPP__

#include <stdexcept>
#include <string>

namespace pdif {

class pdif_not_implemented : public std::exception {
public:

    pdif_not_implemented(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Not Implemented: " + m_msg;}

    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;
};

}

#endif // __PDIF_ERRORS_HPP__