#include "config_panel.hpp"
#include "imgui.h"

namespace plot_genius {

ConfigPanel::ConfigPanel() = default;

void ConfigPanel::Render() {
    if (ImGui::Begin("Configuration")) {
        DrawGridSettings();
        ImGui::Separator();
        DrawAppearanceSettings();
    }
    ImGui::End();
}

void ConfigPanel::SetConfig(const GraphConfig& config) {
    m_config = config;
}

void ConfigPanel::SetConfigCallback(std::function<void(const GraphConfig&)> callback) {
    m_configCallback = std::move(callback);
}

void ConfigPanel::DrawGridSettings() {
    ImGui::Text("Grid Settings");
    if (ImGui::Checkbox("Show Grid", &m_config.showGrid)) {
        if (m_configCallback) {
            m_configCallback(m_config);
        }
    }
    if (ImGui::SliderFloat("Grid Spacing", &m_config.gridSpacing, 0.1f, 5.0f)) {
        if (m_configCallback) {
            m_configCallback(m_config);
        }
    }
}

void ConfigPanel::DrawAppearanceSettings() {
    ImGui::Text("Appearance");
    if (ImGui::SliderFloat("Line Thickness", &m_config.lineThickness, 0.5f, 5.0f)) {
        if (m_configCallback) {
            m_configCallback(m_config);
        }
    }
    if (ImGui::ColorEdit4("Background Color", m_config.backgroundColor)) {
        if (m_configCallback) {
            m_configCallback(m_config);
        }
    }
    if (ImGui::ColorEdit4("Line Color", m_config.lineColor)) {
        if (m_configCallback) {
            m_configCallback(m_config);
        }
    }
}

} // namespace plot_genius 