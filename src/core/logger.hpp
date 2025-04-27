/**
 * Logger System Header
 * 
 * Defines a thread-safe singleton logger system that supports multiple log levels
 * and can output to both console and file.
 */

#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <mutex>

namespace plot_genius {
namespace core {

/**
 * Enumeration of available log severity levels
 */
enum class LogLevel {
    Debug,   ///< Detailed information for debugging purposes
    Info,    ///< General information about system operation
    Warning, ///< Potential issues that don't prevent normal operation
    Error    ///< Serious problems that prevent normal operation
};

/**
 * Thread-safe singleton logger class
 */
class Logger {
public:
    /**
     * Returns the singleton instance of the logger
     * 
     * @return Reference to the logger instance
     */
    static Logger& GetInstance();
    
    /**
     * Sets the output file for logging
     * 
     * @param filename Path to the log file
     */
    void SetLogFile(const std::string& filename);
    
    /**
     * Logs a message with the specified severity level
     * 
     * @param level Severity level of the message
     * @param message Content to log
     */
    void Log(LogLevel level, const std::string& message);
    
    /**
     * Logs a formatted debug message
     * 
     * @param format Format string
     * @param args Arguments to format into the string
     */
    template<typename... Args>
    void Debug(const std::string& format, Args... args) {
        Log(LogLevel::Debug, FormatString(format, args...));
    }
    
    /**
     * Logs a formatted informational message
     * 
     * @param format Format string
     * @param args Arguments to format into the string
     */
    template<typename... Args>
    void Info(const std::string& format, Args... args) {
        Log(LogLevel::Info, FormatString(format, args...));
    }
    
    /**
     * Logs a formatted warning message
     * 
     * @param format Format string
     * @param args Arguments to format into the string
     */
    template<typename... Args>
    void Warning(const std::string& format, Args... args) {
        Log(LogLevel::Warning, FormatString(format, args...));
    }
    
    /**
     * Logs a formatted error message
     * 
     * @param format Format string
     * @param args Arguments to format into the string
     */
    template<typename... Args>
    void Error(const std::string& format, Args... args) {
        Log(LogLevel::Error, FormatString(format, args...));
    }

private:
    /**
     * Private constructor for singleton pattern
     */
    Logger();
    
    /**
     * Private destructor for singleton pattern
     */
    ~Logger();
    
    /**
     * Base case for template recursion in string formatting
     * 
     * @param format Format string
     * @return Formatted string
     */
    std::string FormatString(const std::string& format);
    
    /**
     * Recursive template function for string formatting
     * 
     * @param format Format string
     * @param value Current value to format
     * @param args Remaining arguments
     * @return Formatted string
     */
    template<typename T, typename... Args>
    std::string FormatString(const std::string& format, T value, Args... args);
    
    std::ofstream m_logFile;   ///< File stream for log output
    std::mutex m_mutex;        ///< Mutex for thread safety
    bool m_useFile;            ///< Flag indicating if file logging is enabled
};

} // namespace core
} // namespace plot_genius 