#ifndef __PDIF_ERRORS_HPP__
#define __PDIF_ERRORS_HPP__

#include <stdexcept>
#include <string>

namespace pdif {

/**
 * @brief pdif_not_implemented is thrown when a function is called that is not implemented.
 * 
 */
class pdif_not_implemented : public std::exception {
public:

    /**
     * @brief Construct a new pdif not implemented object
     * 
     * @param t_msg the message to be displayed
     */
    pdif_not_implemented(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Not Implemented: " + m_msg;}

    /**
     * @brief override of std::exception::what()
     * 
     * @return const char* the message to be displayed
     */
    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;
};

/**
 * @brief pdif_out_of_bounds is thrown when an index is out of bounds.
 * 
 */
class pdif_out_of_bounds : public std::exception {
public:

    /**
     * @brief Construct a new pdif out of bounds object
     * 
     * @param t_msg the message to be displayed
     */
    pdif_out_of_bounds(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Out of Bounds: " + m_msg;}

    /**
     * @brief override of std::exception::what()
     * 
     * @return const char* the message to be displayed
     */
    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;
};

/**
 * @brief pdif_invalid_conversion is thrown when a conversion is attempted that is not valid.
 * 
 */
class pdif_invalid_conversion : public std::exception {
public:

    /**
     * @brief Construct a new pdif invalid conversion object
     * 
     * @param t_msg the message to be displayed
     */
    pdif_invalid_conversion(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Invalid Conversion: " + m_msg;}

    /**
     * @brief override of std::exception::what()
     * 
     * @return const char* the message to be displayed
     */
    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;
};

/**
 * @brief pdif_invalid_argment is thrown when an invalid argument is passed to a function.
 * 
 */
class pdif_invalid_argment : public std::exception {
public:

    /**
     * @brief Construct a new pdif invalid argment object
     * 
     * @param t_msg the message to be displayed
     */
    pdif_invalid_argment(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Invalid Argument: " + m_msg;}

    /**
     * @brief override of std::exception::what()
     * 
     * @return const char* the message to be displayed
     */
    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;

};

/**
 * @brief pdif_invalid_key is thrown when an invalid key is passed to a function.
 * 
 */
class pdif_invalid_key : public std::exception {
public:

    /**
     * @brief Construct a new pdif invalid key object
     * 
     * @param t_msg the message to be displayed
     */
    pdif_invalid_key(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Invalid Key: " + m_msg;}

    /**
     * @brief override of std::exception::what()
     * 
     * @return const char* the message to be displayed
     */
    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;

};

/**
 * @brief pdif_error_in_callback is thrown when an error occurs in a callback.
 * 
 */
class pdif_error_in_callback : public std::exception {
public:

    /**
     * @brief Construct a new pdif error in callback object
     * 
     * @param t_msg the message to be displayed
     */
    pdif_error_in_callback(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Error In Callback: " + m_msg;}

    /**
     * @brief override of std::exception::what()
     * 
     * @return const char* the message to be displayed
     */
    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;

};

/**
 * @brief pdif_invalid_operation is thrown when an invalid operation is attempted.
 * 
 */
class pdif_invalid_operation : public std::exception {
public:

    /**
     * @brief Construct a new pdif inavlid operation object
     * 
     * @param t_msg the message to be displayed
     */
    pdif_invalid_operation(const std::string& t_msg) : m_msg(t_msg) {m_msg = "PDIF Invalid Operation: " + m_msg;}

    /**
     * @brief override of std::exception::what()
     * 
     * @return const char* the message to be displayed
     */
    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:

    std::string m_msg;

};
}

#endif // __PDIF_ERRORS_HPP__