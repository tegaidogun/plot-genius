#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>

namespace plot_genius {
namespace config {

class Config {
public:
    static Config& GetInstance();

    // Graph settings
    struct GraphSettings {
        double defaultXMin = -10.0;
        double defaultXMax = 10.0;
        int defaultNumPoints = 100;
        bool showGrid = true;
        std::string lineColor = "#FF0000";
        float lineWidth = 2.0f;
    };

    // UI settings
    struct UISettings {
        int windowWidth = 800;
        int windowHeight = 600;
        std::string theme = "dark";
        bool showFPS = true;
    };

    // Getters
    GraphSettings GetGraphSettings() const;
    UISettings GetUISettings() const;

    // Setters
    void SetGraphSettings(const GraphSettings& settings);
    void SetUISettings(const UISettings& settings);

    // Config file operations
    bool LoadFromFile(const std::string& filename = "config.txt");
    bool SaveToFile(const std::string& filename = "config.txt");

private:
    Config();
    ~Config() = default;

    // Prevent copying
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // Internal helper methods
    void ParseConfigLine(const std::string& line);
    std::string FormatConfigLine(const std::string& key, const std::string& value) const;
    void UpdateSettingsFromMap();
    void UpdateMapFromSettings();

    GraphSettings m_graphSettings;
    UISettings m_uiSettings;
    std::map<std::string, std::string> m_configMap;
    mutable std::mutex m_mutex;
};

} // namespace config
} // namespace plot_genius 