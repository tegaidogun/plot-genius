#pragma once

#include <functional>

namespace plot_genius {

struct GraphConfig {
    bool showGrid{true};
    float gridSpacing{1.0f};
    float lineThickness{1.0f};
    float backgroundColor[4]{0.1f, 0.1f, 0.1f, 1.0f};
    float lineColor[4]{0.0f, 1.0f, 0.0f, 1.0f};
};

class ConfigPanel {
public:
    ConfigPanel();
    ~ConfigPanel() = default;

    void Render();
    void SetConfig(const GraphConfig& config);
    void SetConfigCallback(std::function<void(const GraphConfig&)> callback);

private:
    GraphConfig m_config;
    std::function<void(const GraphConfig&)> m_configCallback;

    void DrawGridSettings();
    void DrawAppearanceSettings();
}; 