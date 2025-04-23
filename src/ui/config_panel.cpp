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
    m_defaultConfig.yAxisScaling = 0.16f;
    m_defaultConfig.backgroundColor = {0.08f, 0.08f, 0.08f, 1.0f};
    m_defaultConfig.gridColor = {0.3f, 0.3f, 0.3f, 1.0f};
    m_defaultConfig.axisColor = {0.5f, 0.5f, 0.5f, 1.0f};
    m_defaultConfig.graphColor = {0.0f, 0.8f, 0.2f, 1.0f};
    
    // Set current config to defaults
    m_config = m_defaultConfig;
}

void ConfigPanel::Render() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_None;
    
    if (ImGui::Begin("Configuration", nullptr, flags)) {
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
        
        if (ImGui::CollapsingHeader("Sensitivity", ImGuiTreeNodeFlags_DefaultOpen)) {
            DrawSensitivitySettings();
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
    ImGui::End();
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
    if (ImGui::ColorEdit3("##GridColor", &m_config.gridColor.x)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Axis color picker
    ImGui::Text("Axis Color");
    ImGui::PushItemWidth(-1);
    if (ImGui::ColorEdit3("##AxisColor", &m_config.axisColor.x)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Graph color picker
    ImGui::Text("Graph Color");
    ImGui::PushItemWidth(-1);
    if (ImGui::ColorEdit3("##GraphColor", &m_config.graphColor.x)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Background color picker
    ImGui::Text("Background Color");
    ImGui::PushItemWidth(-1);
    if (ImGui::ColorEdit3("##BgColor", &m_config.backgroundColor.x)) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
}

void ConfigPanel::DrawSensitivitySettings() {
    // X-axis scaling
    ImGui::Text("X-Axis Scaling");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##XAxisScaling", &m_config.xAxisScaling, 0.1f, 2.0f, "%.2f")) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Y-axis scaling
    ImGui::Text("Y-Axis Scaling");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##YAxisScaling", &m_config.yAxisScaling, 0.01f, 0.5f, "%.2f")) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Pan sensitivity
    ImGui::Text("Pan Sensitivity");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##PanSensitivity", &m_config.panSensitivity, 0.1f, 3.0f, "%.1f")) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Zoom sensitivity
    ImGui::Text("Zoom Sensitivity");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##ZoomSensitivity", &m_config.zoomSensitivity, 0.1f, 3.0f, "%.1f")) {
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
    
    // Display default view range
    ImGui::Text("Default View Range");
    ImGui::Text("X: [%.1f, %.1f]", -m_config.defaultViewWidth/2, m_config.defaultViewWidth/2);
    ImGui::Text("Y: [%.1f, %.1f]", -m_config.defaultViewHeight/2, m_config.defaultViewHeight/2);
    
    // Default view width slider
    ImGui::Text("Default View Width");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##DefaultWidth", &m_config.defaultViewWidth, 1.0f, 50.0f, "%.1f")) {
        // Config will be updated at the end of Render
    }
    ImGui::PopItemWidth();
    
    // Default view height slider
    ImGui::Text("Default View Height");
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##DefaultHeight", &m_config.defaultViewHeight, 0.2f, 10.0f, "%.1f")) {
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