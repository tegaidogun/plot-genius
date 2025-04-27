/**
 * Logger System Implementation
 * 
 * Implements a thread-safe singleton logger that supports console and file output
 * with timestamp and severity level formatting.
 */

#include "logger.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace plot_genius {
namespace core {

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : m_useFile(false) {}

Logger::~Logger() {
    if (m_useFile) {
        m_logFile.close();
    }
}

void Logger::SetLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_useFile) {
        m_logFile.close();
    }
    m_logFile.open(filename, std::ios::app);
    m_useFile = m_logFile.is_open();
}

void Logger::Log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Get current time
    auto now = std::time(nullptr);
    auto tm = std::localtime(&now);
    
    // Format timestamp
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    
    // Format log level
    const char* levelStr = "";
    switch (level) {
        case LogLevel::Debug:   levelStr = "DEBUG"; break;
        case LogLevel::Info:    levelStr = "INFO"; break;
        case LogLevel::Warning: levelStr = "WARNING"; break;
        case LogLevel::Error:   levelStr = "ERROR"; break;
    }
    
    // Format final message
    std::string formatted = "[" + ss.str() + "] [" + levelStr + "] " + message + "\n";
    
    // Output to console
    std::cout << formatted;
    
    // Output to file if enabled
    if (m_useFile) {
        m_logFile << formatted;
        m_logFile.flush();
    }
}

/**
 * Base case implementation for the template string formatter
 */
std::string Logger::FormatString(const std::string& format) {
    return format;
}

/**
 * Recursive implementation of the string formatter
 * Replaces {} placeholders with string representations of values
 */
template<typename T, typename... Args>
std::string Logger::FormatString(const std::string& format, T value, Args... args) {
    std::string result;
    std::size_t pos = format.find("{}");
    if (pos != std::string::npos) {
        std::stringstream ss;
        ss << value;
        result = format.substr(0, pos) + ss.str() + FormatString(format.substr(pos + 2), args...);
    } else {
        result = format;
    }
    return result;
}

// Explicit template instantiations for common types
template std::string Logger::FormatString<double, const char*>(const std::string&, double, const char*);
template std::string Logger::FormatString<std::size_t, std::string>(const std::string&, std::size_t, std::string);
template std::string Logger::FormatString<const char*>(const std::string&, const char*);
template std::string Logger::FormatString<int>(const std::string&, int);
template std::string Logger::FormatString<double>(const std::string&, double);
template std::string Logger::FormatString<float>(const std::string&, float);
template std::string Logger::FormatString<std::string>(const std::string&, std::string);

} // namespace core
} // namespace plot_genius 