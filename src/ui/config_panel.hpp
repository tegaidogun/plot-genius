#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <imgui.h>

namespace plot_genius {

struct GraphConfig {
    // Grid settings
    float gridSpacing{1.0f};
    float lineThickness{2.0f};
    bool showGrid{true};
    
    // Default view dimensions (for reset)
    float defaultViewScaling{20.0f};  // Single scaling factor for both dimensions
    
    // Axis scaling (sensitivity) - fixed values, no longer adjustable in UI
    float xAxisScaling{1.0f};   // Set to 1.0 as per requirement
    float yAxisScaling{0.01f};  // Set to 0.01 as per requirement
    
    // Sensitivity settings - fixed values, no longer adjustable in UI
    float panSensitivity{1.0f};
    float zoomSensitivity{1.0f};
    float xSensitivity{1.0f};
    
    // Colors
    ImVec4 backgroundColor{0.08f, 0.08f, 0.08f, 1.0f};
    ImVec4 gridColor{0.3f, 0.3f, 0.3f, 1.0f};
    ImVec4 axisColor{0.5f, 0.5f, 0.5f, 1.0f};
    ImVec4 graphColor{0.0f, 0.8f, 0.2f, 1.0f};
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
    void DrawViewportSettings();
    
    GraphConfig m_config;
    GraphConfig m_defaultConfig;  // Store default values for reset
    std::function<void(const GraphConfig&)> m_configCallback;
    bool m_resetGraphView{false};
};

} // namespace plot_genius 