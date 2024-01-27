#ifndef __PDIF_LOGGER_HPP__
#define __PDIF_LOGGER_HPP__

#include <util/logger.hpp>
#include <util/memory.hpp>

namespace pdif {

/**
 * @brief simple wrapper that provides an instance for the util::logger class
 * 
 */
class pdif_logger {
public:

    /**
     * @brief return the single instance of the logger
     * 
     * @return util::ref<util::logger> the logger instance
     */
    static util::ref<util::logger> instance();

    /**
     * @brief delete the default constructor
     * 
     */
    pdif_logger() = delete;

private:

    /**
     * @brief static instance of the logger
     * 
     */
    static util::ref<util::logger> logger;
};

/**
 * @brief macros for logging
 */
#define PDIF_LOG_TRACE(...) pdif::pdif_logger::instance()->logTrace(__VA_ARGS__)
#define PDIF_LOG_DEBUG(...) pdif::pdif_logger::instance()->logDebug(__VA_ARGS__)
#define PDIF_LOG_INFO(...) pdif::pdif_logger::instance()->logInfo(__VA_ARGS__)
#define PDIF_LOG_WARN(...) pdif::pdif_logger::instance()->logWarning(__VA_ARGS__)
#define PDIF_LOG_ERROR(...) pdif::pdif_logger::instance()->logError(__VA_ARGS__)
#define PDIF_LOG_CRITICAL(...) pdif::pdif_logger::instance()->logCritical(__VA_ARGS__)

}


#endif // __PDIF_LOGGER_HPP__