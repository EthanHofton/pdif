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
    font_set = 1,
    text_color_set = 2,
    stroke_color_set = 3,
    xobject_image = 4,
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
        case stream_type::font_set:
            os << "font_set";
            break;
        case stream_type::text_color_set:
            os << "text_color_set";
            break;
        case stream_type::stroke_color_set:
            os << "stroke_color_set";
            break;
        case stream_type::xobject_image:
            os << "xobject_image";
            break;
    }
    return os;
}

inline std::istream& operator>>(std::istream& is, stream_type& type) {
    std::string str;
    is >> str;

    if (str == "text") {
        type = stream_type::text;
    } else if (str == "font_set") {
        type = stream_type::font_set;
    } else if (str == "text_color_set") {
        type = stream_type::text_color_set;
    } else if (str == "stroke_color_set") {
        type = stream_type::stroke_color_set;
    } else if (str == "xobject_image") {
            type = stream_type::xobject_image;
    } else {
        PDIF_LOG_ERROR("stream_type::operator>> - invalid stream_type");
        throw pdif::pdif_invalid_argment("stream_type::operator>> - invalid stream_type");
    }

    return is;
}

// forward declarations
class stream_elem;
class text_elem;
class font_elem;
class text_color_elem;
class stroke_color_elem;
class xobject_img_elem;

// typedefs
using rstream_elem = util::ref<stream_elem>;
using rtext_elem = util::ref<text_elem>;
using rfont_elem = util::ref<font_elem>;
using rtext_color_elem = util::ref<text_color_elem>;
using rstroke_color_elem = util::ref<stroke_color_elem>;
using rxobject_img_elem = util::ref<xobject_img_elem>;

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
     * @brief Destroy the stream elem object with a virtual destructor
     * 
     */
    virtual ~stream_elem() = default;

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
 * @brief A concrete subclass of stream_elem that represents a font change
 * 
 */
class font_elem : public stream_elem {
public:

    /**
     * @brief static type method for font_elem
     * 
     */
    STATIC_TYPE(stream_type::font_set)

    /**
     * @brief Construct a new font elem object
     * 
     * @param t_font_name the name of the font
     * @param t_font_size the size of the font
     * @param t the private_tag to allow construction
     */
    font_elem(stream_elem::private_tag, const std::string& t_font_name, int t_font_size);

    /**
     * @brief getter for the font name
     * 
     * @return const std::string& the name of the font
     */
    const std::string& font_name() const;
    /**
     * @brief getter for the font size
     * 
     * @return int the size of the font
     */
    int font_size() const;

    /**
     * @brief the implementation of stream_elem::compare
     * 
     * @param t_other the other stream_elem to compare to
     * @return true if the types are the same and the font name and size are equal
     * @return false otherwise
     */
    virtual bool compare(rstream_elem t_other) override;

    /**
     * @brief return the stringified font_elem
     * 
     * @return std::string the stringified font_elem
     */
    virtual std::string to_string() const override;

private:

    std::string m_font_name;
    int m_font_size;
};

/**
 * @brief A concrete subclass of stream_elem that represents a color change
 * 
 */
class color_elem : public stream_elem {
public:

    /**
     * @brief Construct a new text color elem object
     * 
     * @param t_color the color to set
     * @param t the private_tag to allow construction
     */
    color_elem(stream_elem::private_tag, stream_type t_type, int t_r, int t_g, int t_b) : stream_elem(private_tag(), t_type), r(t_r), g(t_g), b(t_b) {}

    /**
     * @brief Get the red value of the color
     * 
     * @return int
     */
    inline int red() const { return r; }
    /**
     * @brief Get the green value of the color
     * 
     * @return int 
     */
    inline int green() const { return g; }
    /**
     * @brief Get the blue value of the color
     * 
     * @return int 
     */
    inline int blue() const { return b; }

protected:

    int r;
    int g;
    int b;
};

/**
 * @brief A concrete subclass of stream_elem that represents a text color change
 * 
 */
class text_color_elem : public color_elem {
public:

    /**
     * @brief static type method for text_color_elem
     * 
     */
    STATIC_TYPE(stream_type::text_color_set)

    /**
     * @brief Construct a new text color elem object
     * 
     * @param t_color the color to set
     * @param t the private_tag to allow construction
     */
    text_color_elem(stream_elem::private_tag, int t_r, int t_g, int t_b);

    /**
     * @brief the implementation of stream_elem::compare
     * 
     * @param t_other the other stream_elem to compare to
     * @return true if the types are the same and the colors are equal
     * @return false otherwise
     */
    virtual bool compare(rstream_elem t_other) override;

    /**
     * @brief return the stringified text_color_elem
     * 
     * @return std::string the stringified text_color_elem
     */
    virtual std::string to_string() const override;
};

/**
 * @brief A concrete subclass of stream_elem that represents a stroke color change
 * 
 */
class stroke_color_elem : public color_elem {
public:

    /**
     * @brief static type method for stroke_color_elem
     * 
     */
    STATIC_TYPE(stream_type::stroke_color_set)

    /**
     * @brief Construct a new stroke color elem object
     * 
     * @param t_color the color to set
     * @param t the private_tag to allow construction
     */
    stroke_color_elem(stream_elem::private_tag, int t_r, int t_g, int t_b);

    /**
     * @brief the implementation of stream_elem::compare
     * 
     * @param t_other the other stream_elem to compare to
     * @return true if the types are the same and the colors are equal
     * @return false otherwise
     */
    virtual bool compare(rstream_elem t_other) override;

    /**
     * @brief return the stringified stroke_color_elem
     * 
     * @return std::string the stringified stroke_color_elem
     */
    virtual std::string to_string() const override;
};

/**
 * @brief A concrete subclass of stream_elem that represents an XObject image
 * 
 */
class xobject_img_elem : public stream_elem {
public:

    /**
     * @brief A static type method for xobject_img_elem
     * 
     */
    STATIC_TYPE(stream_type::xobject_image)

    /**
     * @brief Construct a new xobject img elem object
     * 
     * @param t_image_hash the image hash
     * @param t_width the width
     * @param t_height the height
     */
    xobject_img_elem(stream_elem::private_tag, const std::string& t_image_hash, int t_width, int t_height);

    /**
     * @brief get the image hash
     * 
     * @return const std::string& 
     */
    inline const std::string& image_hash() const { return m_image_hash; }
    /**
     * @brief get the width
     * 
     * @return int 
     */
    inline int width() const { return m_width; }
    /**
     * @brief get the height
     * 
     * @return int 
     */
    inline int height() const { return m_height; }

    /**
     * @brief Compare this xobject_img_elem to another stream_elem
     * 
     * @param t_other the other
     * @return true 
     * @return false 
     */
    virtual bool compare(rstream_elem t_other) override;

    /**
     * @brief Convert this xobject_img_elem to a string
     * 
     * @return std::string
     */
    virtual std::string to_string() const override;

private:

    std::string m_image_hash;
    int m_width;
    int m_height;

};

}

#endif // __PDIF_STREAM_ELEM_HPP__