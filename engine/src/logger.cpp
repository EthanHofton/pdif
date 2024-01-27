#include <pdif/logger.hpp>
#include <util/logger_console_sink.hpp>

namespace pdif {

util::ref<util::logger> pdif_logger::logger = nullptr;

util::ref<util::logger> pdif_logger::instance() {
    if (logger == nullptr) {
        // initialize logger
        logger = util::create_ref<util::logger>();

        // add logger sinks
        logger->addSink<util::logger_console_sink>("pdif_console");
    }

    return logger;
}

}