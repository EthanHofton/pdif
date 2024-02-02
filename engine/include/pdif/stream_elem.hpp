#ifndef __PDIF_STREAM_ELEM_HPP__
#define __PDIF_STREAM_ELEM_HPP__

#include <util/memory.hpp>
#include <string>
#include <vector>

#include <pdif/errors.hpp>
#include <pdif/logger.hpp>

namespace pdif {

/**
 * @brief enum class for the type of stream_elem
 * 
 */
enum class stream_type {
    text = 0,
    binary = 1,
};

/**
 * @brief << operator for stream_type
 * 
 * @param os the ostream to write to
 * @param type the stream_type to write
 * @return std::ostream& the ostream
 */
inline std::ostream& operator<<(std::ostream& os, const stream_type& type) {
    switch (type) {
        case stream_type::text:
            os << "text";
            break;
        case stream_type::binary:
            os << "binary";
            break;
    }
    return os;
}

// forward declarations
class stream_elem;
class text_elem;
class binary_elem;

// typedefs
using rstream_elem = util::ref<stream_elem>;
using rtext_elem = util::ref<text_elem>;
using rbinary_elem = util::ref<binary_elem>;

/**
 * @brief macro to create a static type function for a stream_elem subclass
 * 
 */
#define STATIC_TYPE(type) inline static stream_type static_type() { return type; }

/**
 * @brief base class for all stream elements, inherits from std::enable_shared_from_this to allow for shared_from_this() calls
 * 
 */
class stream_elem : public std::enable_shared_from_this<stream_elem> {
protected:

    /**
     * @brief private to prevent the creation of stream_elem objects, only allow creation through the create function
     * 
     */
    struct private_tag {};

public:

    /*
    * @brief creates a stream_elem with the given type
    * @param t_type the type of the stream_elem 
    * 
    * this constructor is private to prevent the creation of stream_elem objects, only allow creation through the create function
    */
    stream_elem(private_tag, stream_type t_type);

    /**
     * @brief compare this stream_elem to another stream_elem
     * 
     * @param t_other the other stream_elem to compare to
     * @return true if the stream_elems are equal
     * @return false if the stream_elems are not equal
     */
    virtual bool compare(rstream_elem t_other) = 0;
    /**
     * @brief returns the type of the stream_elem
     * 
     * @return stream_type the type of the stream_elem
     */
    stream_type type() const;

    /**
     * @brief stringify the stream_elem
     * 
     * @return std::string the stringified stream_elem
     */
    virtual std::string to_string() const = 0;

    /**
     * @brief 
     * 
     * @tparam T the type of the stream_elem to create
     * @tparam Args the types of arguments to pass to the constructor of the stream_elem
     * @param args the arguments to pass to the constructor of the stream_elem
     * @return rstream_elem a reference to the newly created stream_elem
     *
     * metaprogramming is used to ensure that the type T is a subclass of stream_elem
     */
    template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<stream_elem, T>>>
    static inline rstream_elem create(Args&&... args) {
        return util::create_ref<T>(private_tag(), std::forward<Args>(args)...);
    }

    /**
     * @brief return a reference to this stream_elem as a stream_elem of type T
     * 
     * @tparam T the type of the stream_elem to return
     * @return util::ref<T> a reference to this stream_elem as a stream_elem of type T
     * 
     * metaprogramming is used to ensure that the type T is a subclass of stream_elem
     */
    template<typename T, typename = std::enable_if_t<std::is_base_of_v<stream_elem, T>>>
    inline util::ref<T> as() {
        // check type
        if (type() != T::static_type()) {
            PDIF_LOG_ERROR("cannot convert stream_elem of type {} to type {}", static_cast<int>(type()), static_cast<int>(T::static_type()));
            throw pdif::pdif_invalid_conversion("cannot convert stream_elem of type " + std::to_string(static_cast<int>(type())) + " to type " + std::to_string(static_cast<int>(T::static_type())));
        }

        // cast the shared_ptr to a shared_ptr of type T and return it
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

private:

    stream_type m_type;

};

/**
 * @brief a concrete subclass of stream_elem that holds a string
 * 
 */
class text_elem : public stream_elem {
public:

    /**
     * @brief static type method for text_elem
     * 
     */
    STATIC_TYPE(stream_type::text)

    /**
     * @brief Construct a new text elem object
     * 
     * @param t the private_tag to allow construction 
     * @param t_text the text to store in the text_elem
     */
    text_elem(stream_elem::private_tag, const std::string& t_text);

    /**
     * @brief text getter
     * 
     * @return const std::string& the text stored in the text_elem
     */
    const std::string& text() const;
    /**
     * @brief implementation of stream_elem::compare
     * 
     * @param t_other the other stream_elem to compare to
     * @return true if the stream_elems are of the same type and have the same text
     * @return false otherwise
     */
    virtual bool compare(rstream_elem t_other) override;

    /**
     * @brief return the stringified text_elem
     * 
     * @return std::string the stringified text_elem
     */
    virtual std::string to_string() const override;

private:

    std::string m_text;

};

/**
 * @brief a concrete subclass of stream_elem that holds a vector of chars
 * 
 */
class binary_elem : public stream_elem {
public:

    /**
     * @brief static type method for binary_elem
     * 
     */
    STATIC_TYPE(stream_type::binary)

    /**
     * @brief Construct a new binary elem object
     * 
     * @param t_binary the binary to store in the binary_elem
     * @param t the private_tag to allow construction
     */
    binary_elem(stream_elem::private_tag, const std::vector<char>& t_binary);

    /**
     * @brief getter for the binary
     * 
     * @return const std::vector<char>& the binary stored in the binary_elem
     */
    const std::vector<char>& binary() const;
    /**
     * @brief the implementation of stream_elem::compare
     * 
     * @param t_other the other stream_elem to compare to
     * @return true the types are the same and the binary is equal (bitwise)
     * @return false otherwise
     */
    virtual bool compare(rstream_elem t_other) override;

    /**
     * @brief return the stringified binary_elem
     * 
     * @return std::string the stringified binary_elem
     */
    virtual std::string to_string() const override;

private:

    std::vector<char> m_binary;
};


}


#endif // __PDIF_STREAM_ELEM_HPP__