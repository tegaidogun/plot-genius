#pragma once

#include <glm/glm.hpp>
#include <functional>

namespace plot_genius {

struct GraphConfig {
    // Grid settings
    float gridSpacing{1.0f};
    float lineThickness{2.0f};
    bool showGrid{true};
    
    // Default view dimensions (for reset)
    float defaultViewWidth{20.0f};    // -10 to 10 on X axis
    float defaultViewHeight{3.2f};    // -1.6 to 1.6 on Y axis with scaling
    
    // Axis scaling (sensitivity)
    float xAxisScaling{1.0f};   // Range: 0.1f - 2.0f, Default: 1.0f
    float yAxisScaling{0.16f};  // Range: 0.1f - 2.0f, Default: 0.16f
    
    // Sensitivity settings
    float panSensitivity{1.0f};
    float zoomSensitivity{1.0f};
    float xSensitivity{1.0f};   // New setting for X-axis sensitivity
    
    // Colors
    glm::vec4 backgroundColor{0.08f, 0.08f, 0.08f, 1.0f};
    glm::vec4 gridColor{0.3f, 0.3f, 0.3f, 1.0f};
    glm::vec4 axisColor{0.5f, 0.5f, 0.5f, 1.0f};
    glm::vec4 graphColor{0.0f, 0.8f, 0.2f, 1.0f};
};

class ConfigPanel {
public:
    ConfigPanel();
    
    void Render();
    
    void SetConfig(const GraphConfig& config);
    void SetConfigCallback(std::function<void(const GraphConfig&)> callback);
    
    bool ShouldResetGraphView() const { return m_resetGraphView; }
    void ClearResetFlag() { m_resetGraphView = false; }
    
private:
    void DrawGridSettings();
    void DrawAppearanceSettings();
    void DrawSensitivitySettings();
    void DrawViewportSettings();
    
    GraphConfig m_config;
    GraphConfig m_defaultConfig;  // Store default values for reset
    std::function<void(const GraphConfig&)> m_configCallback;
    bool m_resetGraphView{false};
};

} // namespace plot_genius 