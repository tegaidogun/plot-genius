#include "config.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace plot_genius {
namespace config {

Config& Config::GetInstance() {
    static Config instance;
    return instance;
}

Config::Config() {
    // Initialize default settings
    m_graphSettings = GraphSettings();
    m_uiSettings = UISettings();
    
    // Try to load from default config file
    LoadFromFile();
}

bool Config::LoadFromFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        ParseConfigLine(line);
    }
    
    UpdateSettingsFromMap();
    return true;
}

bool Config::SaveToFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    UpdateMapFromSettings();
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& [key, value] : m_configMap) {
        file << FormatConfigLine(key, value) << '\n';
    }
    
    return true;
}

void Config::ParseConfigLine(const std::string& line) {
    if (line.empty() || line[0] == '#') {
        return;
    }
    
    size_t pos = line.find('=');
    if (pos == std::string::npos) {
        return;
    }
    
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    
    // Trim whitespace
    key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    key.erase(std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), key.end());
    
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), value.end());
    
    m_configMap[key] = value;
}

std::string Config::FormatConfigLine(const std::string& key, const std::string& value) const {
    return key + " = " + value;
}

void Config::UpdateSettingsFromMap() {
    // Graph settings
    if (m_configMap.count("graph.defaultXMin")) {
        m_graphSettings.defaultXMin = std::stod(m_configMap["graph.defaultXMin"]);
    }
    if (m_configMap.count("graph.defaultXMax")) {
        m_graphSettings.defaultXMax = std::stod(m_configMap["graph.defaultXMax"]);
    }
    if (m_configMap.count("graph.defaultNumPoints")) {
        m_graphSettings.defaultNumPoints = std::stoi(m_configMap["graph.defaultNumPoints"]);
    }
    if (m_configMap.count("graph.showGrid")) {
        m_graphSettings.showGrid = (m_configMap["graph.showGrid"] == "true");
    }
    if (m_configMap.count("graph.lineColor")) {
        m_graphSettings.lineColor = m_configMap["graph.lineColor"];
    }
    if (m_configMap.count("graph.lineWidth")) {
        m_graphSettings.lineWidth = std::stof(m_configMap["graph.lineWidth"]);
    }
    
    // UI settings
    if (m_configMap.count("ui.windowWidth")) {
        m_uiSettings.windowWidth = std::stoi(m_configMap["ui.windowWidth"]);
    }
    if (m_configMap.count("ui.windowHeight")) {
        m_uiSettings.windowHeight = std::stoi(m_configMap["ui.windowHeight"]);
    }
    if (m_configMap.count("ui.theme")) {
        m_uiSettings.theme = m_configMap["ui.theme"];
    }
    if (m_configMap.count("ui.showFPS")) {
        m_uiSettings.showFPS = (m_configMap["ui.showFPS"] == "true");
    }
}

void Config::UpdateMapFromSettings() {
    // Graph settings
    m_configMap["graph.defaultXMin"] = std::to_string(m_graphSettings.defaultXMin);
    m_configMap["graph.defaultXMax"] = std::to_string(m_graphSettings.defaultXMax);
    m_configMap["graph.defaultNumPoints"] = std::to_string(m_graphSettings.defaultNumPoints);
    m_configMap["graph.showGrid"] = m_graphSettings.showGrid ? "true" : "false";
    m_configMap["graph.lineColor"] = m_graphSettings.lineColor;
    m_configMap["graph.lineWidth"] = std::to_string(m_graphSettings.lineWidth);
    
    // UI settings
    m_configMap["ui.windowWidth"] = std::to_string(m_uiSettings.windowWidth);
    m_configMap["ui.windowHeight"] = std::to_string(m_uiSettings.windowHeight);
    m_configMap["ui.theme"] = m_uiSettings.theme;
    m_configMap["ui.showFPS"] = m_uiSettings.showFPS ? "true" : "false";
}

Config::GraphSettings Config::GetGraphSettings() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_graphSettings;
}

Config::UISettings Config::GetUISettings() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_uiSettings;
}

void Config::SetGraphSettings(const GraphSettings& settings) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_graphSettings = settings;
    UpdateMapFromSettings();
}

void Config::SetUISettings(const UISettings& settings) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_uiSettings = settings;
    UpdateMapFromSettings();
}

} // namespace config
} // namespace plot_genius 