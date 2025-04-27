#include "equation_panel.hpp"
#include "imgui.h"
#include <algorithm>
#include <sstream>

namespace plot_genius {

EquationPanel::EquationPanel() : m_hasError(false) {
    memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
}

void EquationPanel::Render() {
    // Main content
    DrawEquationInput();
    ImGui::Separator();
    DrawEquationsList();
    ImGui::Separator();
    DrawHistory();
}

void EquationPanel::SetEquationCallback(std::function<void(const std::string&)> callback) {
    m_equationCallback = std::move(callback);
}

void EquationPanel::SetRemoveCallback(std::function<void(int)> callback) {
    m_removeCallback = std::move(callback);
}

void EquationPanel::DrawEquationInput() {
    ImGui::Text("Enter equation (format: y=f(x)):");
    
    // Input field with buffer
    ImGui::PushItemWidth(-50); // Make room for Add button
    if (ImGui::InputText("##equation", m_inputBuffer, sizeof(m_inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        m_currentEquation = m_inputBuffer;
        
        // Validate and submit
        if (ValidateEquation(m_currentEquation)) {
            m_hasError = false;
            m_errorMessage.clear();
            
            // Add to equations list
            AddEquation(m_currentEquation);
            
            // Add to history if not already there
            if (std::find(m_history.begin(), m_history.end(), m_currentEquation) == m_history.end()) {
                m_history.push_back(m_currentEquation);
                // Keep history limited to 10 items
                if (m_history.size() > 10) {
                    m_history.erase(m_history.begin());
                }
            }
            
            // Clear input field
            memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
        } else {
            // Show error
            m_hasError = true;
        }
    }
    ImGui::PopItemWidth();
    
    // Add button next to input field
    ImGui::SameLine();
    if (ImGui::Button("+")) {
        m_currentEquation = m_inputBuffer;
        
        // Validate and submit
        if (ValidateEquation(m_currentEquation)) {
            m_hasError = false;
            m_errorMessage.clear();
            
            // Add to equations list
            AddEquation(m_currentEquation);
            
            // Add to history if not already there
            if (std::find(m_history.begin(), m_history.end(), m_currentEquation) == m_history.end()) {
                m_history.push_back(m_currentEquation);
                // Keep history limited to 10 items
                if (m_history.size() > 10) {
                    m_history.erase(m_history.begin());
                }
            }
            
            // Clear input field
            memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
        } else {
            // Show error
            m_hasError = true;
        }
    }
    
    // Show error if any
    if (m_hasError && !m_errorMessage.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::TextWrapped("%s", m_errorMessage.c_str());
        ImGui::PopStyleColor();
    }
    
    // Show help
    if (ImGui::Button("Help")) {
        ImGui::OpenPopup("Equation Help");
    }
    
    if (ImGui::BeginPopup("Equation Help")) {
        ImGui::Text("Supported format: y=f(x)");
        ImGui::Text("Examples:");
        ImGui::BulletText("y=x^2");
        ImGui::BulletText("y=sin(x)");
        ImGui::BulletText("y=2*x+5");
        ImGui::Text("\nSupported functions:");
        ImGui::Columns(3, "functions");
        ImGui::BulletText("sin(x)");
        ImGui::BulletText("cos(x)");
        ImGui::BulletText("tan(x)");
        ImGui::NextColumn();
        ImGui::BulletText("sqrt(x)");
        ImGui::BulletText("log(x)");
        ImGui::BulletText("exp(x)");
        ImGui::NextColumn();
        ImGui::BulletText("abs(x)");
        ImGui::BulletText("pow(x,y)");
        ImGui::Columns(1);
        ImGui::Text("\nSupported constants:");
        ImGui::BulletText("pi (3.14159...)");
        ImGui::BulletText("e (2.71828...)");
        ImGui::EndPopup();
    }
}

void EquationPanel::DrawEquationsList() {
    ImGui::Text("Active Equations:");
    
    if (m_equations.empty()) {
        ImGui::TextDisabled("No equations added yet");
        return;
    }
    
    // Display all equations in the list with buttons to remove
    for (auto it = m_equations.begin(); it != m_equations.end();) {
        ImGui::PushID(it->id);
        
        // Checkbox to toggle if the equation is active
        bool isActive = it->isActive;
        if (ImGui::Checkbox("##active", &isActive)) {
            it->isActive = isActive;
            // Update the graph for the active equation
            if (isActive && m_equationCallback) {
                m_equationCallback(it->expression);
            }
        }
        
        ImGui::SameLine();
        
        // Display the equation
        ImGui::Text("%s", it->expression.c_str());
        
        // Add a remove button
        ImGui::SameLine(ImGui::GetWindowWidth() - 30);
        if (ImGui::Button("-")) {
            // Store ID to remove
            int idToRemove = it->id;
            // Remove the equation
            it = m_equations.erase(it);
            // Notify external systems
            RemoveEquation(idToRemove);
            // Skip increment below
            ImGui::PopID();
            continue;
        }
        
        ImGui::PopID();
        ++it;
    }
}

void EquationPanel::DrawHistory() {
    ImGui::Text("History:");
    
    if (m_history.empty()) {
        ImGui::TextDisabled("No equations yet");
        return;
    }
    
    for (const auto& equation : m_history) {
        if (ImGui::Selectable(equation.c_str())) {
            // Copy to input buffer
            strncpy(m_inputBuffer, equation.c_str(), sizeof(m_inputBuffer) - 1);
            m_inputBuffer[sizeof(m_inputBuffer) - 1] = '\0';
        }
    }
}

bool EquationPanel::ValidateEquation(const std::string& equation) {
    // Basic validation: must start with y=
    if (equation.length() < 2 || equation.substr(0, 2) != "y=") {
        m_errorMessage = "Equation must start with 'y='";
        return false;
    }
    
    // More validation can be added here
    
    return true;
}

void EquationPanel::SetCurrentEquation(const std::string& equation) {
    m_currentEquation = equation;
    
    // Set the input buffer
    strncpy(m_inputBuffer, equation.c_str(), sizeof(m_inputBuffer) - 1);
    m_inputBuffer[sizeof(m_inputBuffer) - 1] = '\0';
    
    // Validate and add to history and equations list
    if (ValidateEquation(equation)) {
        // Add to equations list if not already present
        bool equationExists = false;
        for (const auto& eq : m_equations) {
            if (eq.expression == equation) {
                equationExists = true;
                break;
            }
        }
        
        if (!equationExists) {
            AddEquation(equation);
        }
        
        // Add to history if not already there
        if (std::find(m_history.begin(), m_history.end(), equation) == m_history.end()) {
            m_history.push_back(equation);
            // Keep history limited to 10 items
            if (m_history.size() > 10) {
                m_history.erase(m_history.begin());
            }
        }
    }
}

void EquationPanel::AddEquation(const std::string& equation) {
    // Create new equation
    Equation newEquation;
    newEquation.expression = equation;
    newEquation.isActive = true;
    newEquation.id = GetNextEquationId();
    
    // Add to list
    m_equations.push_back(newEquation);
    
    // Call the callback with the new equation
    if (m_equationCallback) {
        m_equationCallback(equation);
    }
}

void EquationPanel::RemoveEquation(int id) {
    // If we removed the last equation, we should handle it appropriately
    // Call the external remove callback if available
    if (m_removeCallback) {
        m_removeCallback(id);
    }
}

int EquationPanel::GetNextEquationId() {
    return m_nextEquationId++;
}

} // namespace plot_genius 