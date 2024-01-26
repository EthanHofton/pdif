#ifndef __PDIF_STREAM_ELEM_HPP__
#define __PDIF_STREAM_ELEM_HPP__

#include <string>
#include <vector>

namespace pdif {

enum class stream_type {
    text = 0,
    binary = 1,
};

class stream_elem {
public:

    stream_elem(const stream_type& t_type);
    virtual bool compare(const stream_elem& t_other) = 0;
    stream_type type() const;

private:

    stream_type m_type;

};

class text_elem : public stream_elem {
public:

    text_elem(const std::string& t_text);

    const std::string& text() const;
    virtual bool compare(const stream_elem& t_other) override;

private:

    std::string m_text;

};

class binary_elem : public stream_elem {
public:

    binary_elem(const std::vector<char>& t_binary);

    const std::vector<char>& binary() const;
    virtual bool compare(const stream_elem& t_other) override;

private:

    std::vector<char> m_binary;
};

}


#endif // __PDIF_STREAM_ELEM_HPP__