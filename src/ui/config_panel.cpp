#include "config_panel.hpp"
#include "../core/logger.hpp"
#include <imgui.h>

namespace plot_genius {

ConfigPanel::ConfigPanel() {
    // Set up default config values
    m_defaultConfig.showGrid = true;
    m_defaultConfig.gridSpacing = 1.0f;
    m_defaultConfig.lineThickness = 2.0f;
    m_defaultConfig.xAxisScaling = 1.0f;
    m_defaultConfig.yAxisScaling = 0.01f;
    m_defaultConfig.backgroundColor = {0.08f, 0.08f, 0.08f, 1.0f};
    m_defaultConfig.gridColor = {0.3f, 0.3f, 0.3f, 1.0f};
    m_defaultConfig.axisColor = {0.5f, 0.5f, 0.5f, 1.0f};
    m_defaultConfig.graphColor = {0.0f, 0.8f, 0.2f, 1.0f};
    m_defaultConfig.defaultViewScaling = 20.0f;
    
    // Set current config to defaults
    m_config = m_defaultConfig;
}

void ConfigPanel::Render() {
    // Render only in the Configuration panel - no separate Graph Settings panel
    bool configChanged = false;
    
    // Create collapsing headers for different config sections
    if (ImGui::CollapsingHeader("Grid Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        DrawGridSettings();
        configChanged = true;
    }
    
    if (ImGui::CollapsingHeader("Appearance", ImGuiTreeNodeFlags_DefaultOpen)) {
        DrawAppearanceSettings();
        configChanged = true;
    }
    
    if (ImGui::CollapsingHeader("View", ImGuiTreeNodeFlags_DefaultOpen)) {
        DrawViewportSettings();
        configChanged = true;
    }
    
    // Apply changes if any setting was modified
    if (configChanged && m_configCallback) {
        m_configCallback(m_config);
    }
}

void ConfigPanel::DrawGridSettings() {
    // Grid visibility
    if (ImGui::Checkbox("Show Grid", &m_config.showGrid)) {
        // Config will be updated at the end of Render
    }
    
    // Grid spacing slider
    ImGui::Text("Grid Spacing");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##GridSpacing", &m_config.gridSpacing, 0.1f, 5.0f, "%.1f")) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Line thickness slider
    ImGui::Text("Line Thickness");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##LineThickness", &m_config.lineThickness, 1.0f, 5.0f, "%.1f")) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
}

void ConfigPanel::DrawAppearanceSettings() {
    // Grid color picker
    ImGui::Text("Grid Color");
    ImGui::PushItemWidth(-1);
    if (ImGui::ColorEdit3("##GridColor", (float*)&m_config.gridColor)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Axis color picker
    ImGui::Text("Axis Color");
    ImGui::PushItemWidth(-1);
    if (ImGui::ColorEdit3("##AxisColor", (float*)&m_config.axisColor)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Graph color picker
    ImGui::Text("Graph Color");
    ImGui::PushItemWidth(-1);
    if (ImGui::ColorEdit3("##GraphColor", (float*)&m_config.graphColor)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Background color picker
    ImGui::Text("Background Color");
    ImGui::PushItemWidth(-1);
    if (ImGui::ColorEdit3("##BgColor", (float*)&m_config.backgroundColor)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
}

void ConfigPanel::DrawViewportSettings() {
    ImGui::Text("Viewport Controls");
    
    // Reset viewport button
    if (ImGui::Button("Reset Viewport", ImVec2(-1, 0))) {
        m_resetGraphView = true;
    }
    
    // Display default view range (now symmetrical)
    float halfSize = m_config.defaultViewScaling / 2.0f;
    ImGui::Text("Default View Range");
    ImGui::Text("X: [%.1f, %.1f]", -halfSize, halfSize);
    ImGui::Text("Y: [%.1f, %.1f]", -halfSize, halfSize);
    
    // Default view scaling slider (single control for both dimensions)
    ImGui::Text("Default View Scaling");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##DefaultScaling", &m_config.defaultViewScaling, 5.0f, 100.0f, "%.1f")) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
}

void ConfigPanel::SetConfig(const GraphConfig& config) {
    m_config = config;
}

void ConfigPanel::SetConfigCallback(std::function<void(const GraphConfig&)> callback) {
    m_configCallback = std::move(callback);
}

} // namespace plot_genius 