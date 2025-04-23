#pragma once

#include <string>
#include <iostream>
#include <sstream>

namespace plot_genius {
namespace util {

class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);
    void Error(const std::string& message);
    
    void Log(const std::string& message);
    
    template<typename... Args>
    static std::string FormatString(const std::string& format, Args&&... args) {
        // Simple implementation that doesn't rely on std::format
        std::stringstream ss;
        FormatStringImpl(ss, format, 0, std::forward<Args>(args)...);
        return ss.str();
    }

private:
    Logger() = default;
    static void Log(Level level, const std::string& message);
    
    template<typename T, typename... Args>
    static void FormatStringImpl(std::stringstream& ss, const std::string& format, 
                                size_t pos, T&& value, Args&&... args) {
        size_t open_brace = format.find('{', pos);
        size_t close_brace = format.find('}', open_brace);
        
        if (open_brace != std::string::npos && close_brace != std::string::npos) {
            ss << format.substr(pos, open_brace - pos);
            ss << value;
            FormatStringImpl(ss, format, close_brace + 1, std::forward<Args>(args)...);
        } else {
            ss << format.substr(pos);
        }
    }
    
    static void FormatStringImpl(std::stringstream& ss, const std::string& format, size_t pos) {
        ss << format.substr(pos);
    }
};

} // namespace util
} // namespace plot_genius 