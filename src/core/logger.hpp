#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <mutex>

namespace plot_genius {
namespace core {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static Logger& GetInstance();
    
    void SetLogFile(const std::string& filename);
    void Log(LogLevel level, const std::string& message);
    
    template<typename... Args>
    void Debug(const std::string& format, Args... args) {
        Log(LogLevel::Debug, FormatString(format, args...));
    }
    
    template<typename... Args>
    void Info(const std::string& format, Args... args) {
        Log(LogLevel::Info, FormatString(format, args...));
    }
    
    template<typename... Args>
    void Warning(const std::string& format, Args... args) {
        Log(LogLevel::Warning, FormatString(format, args...));
    }
    
    template<typename... Args>
    void Error(const std::string& format, Args... args) {
        Log(LogLevel::Error, FormatString(format, args...));
    }

private:
    Logger();
    ~Logger();
    
    std::string FormatString(const std::string& format);
    template<typename T, typename... Args>
    std::string FormatString(const std::string& format, T value, Args... args);
    
    std::ofstream m_logFile;
    std::mutex m_mutex;
    bool m_useFile;
};

} // namespace core
} // namespace plot_genius 