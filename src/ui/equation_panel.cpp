#include "equation_panel.hpp"
#include "imgui.h"

namespace plot_genius {

EquationPanel::EquationPanel() = default;

void EquationPanel::Render() {
    if (ImGui::Begin("Equation")) {
        DrawEquationInput();
        ImGui::Separator();
        DrawHistory();
    }
    ImGui::End();
}

void EquationPanel::SetEquationCallback(std::function<void(const std::string&)> callback) {
    m_equationCallback = std::move(callback);
}

void EquationPanel::DrawEquationInput() {
    ImGui::Text("Enter equation:");
    if (ImGui::InputText("##equation", &m_currentEquation, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (!m_currentEquation.empty()) {
            m_history.push_back(m_currentEquation);
            if (m_equationCallback) {
                m_equationCallback(m_currentEquation);
            }
        }
    }
}

void EquationPanel::DrawHistory() {
    ImGui::Text("History:");
    for (const auto& equation : m_history) {
        ImGui::Text("%s", equation.c_str());
    }
}

} // namespace plot_genius 