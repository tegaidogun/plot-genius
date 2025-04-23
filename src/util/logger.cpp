#include "logger.hpp"

namespace plot_genius {
namespace util {

void Logger::Debug(const std::string& message) {
    Log(Level::DEBUG, message);
}

void Logger::Info(const std::string& message) {
    Log(Level::INFO, message);
}

void Logger::Warning(const std::string& message) {
    Log(Level::WARNING, message);
}

void Logger::Error(const std::string& message) {
    Log(Level::ERROR, message);
}

void Logger::Log(const std::string& message) {
    Log(Level::INFO, message);
}

void Logger::Log(Level level, const std::string& message) {
    std::string prefix;
    
    switch (level) {
        case Level::DEBUG:
            prefix = "[DEBUG] ";
            break;
        case Level::INFO:
            prefix = "[INFO] ";
            break;
        case Level::WARNING:
            prefix = "[WARNING] ";
            break;
        case Level::ERROR:
            prefix = "[ERROR] ";
            break;
    }
    
    std::cout << prefix << message << std::endl;
}

} // namespace util
} // namespace plot_genius 