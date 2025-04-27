#include "graph_panel.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <iostream>
#include <algorithm> // For std::find

namespace plot_genius {

GraphPanel::GraphPanel() {
    m_config = GraphConfig{};
}

void GraphPanel::Render() {
    // Set window flags to ensure visibility
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    
    if (ImGui::Begin("Graph", nullptr, flags)) {
        // Get the canvas size for drawing
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        
        // Make sure we have a minimum size
        if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
        if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;
        
        // Calculate scale factors - this is key to proper scaling
        float scaleX = canvasSize.x / (m_viewMaxX - m_viewMinX);
        float scaleY = canvasSize.y / (m_viewMaxY - m_viewMinY);
        
        // Log position and size for debugging
        static int debugCounter = 0;
        if (debugCounter++ % 100 == 0) {
            ImVec2 pos = ImGui::GetCursorScreenPos();
            std::string debugInfo = "GraphPanel: pos(" + std::to_string(pos.x) + "," + 
                                   std::to_string(pos.y) + ") size(" + 
                                   std::to_string(canvasSize.x) + "," +
                                   std::to_string(canvasSize.y) + ")";
            std::cout << debugInfo << std::endl;
            std::cout << "View range: X[" << m_viewMinX << ", " << m_viewMaxX << "] "
                     << "Y[" << m_viewMinY << ", " << m_viewMaxY << "]" << std::endl;
        }
        
        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        // Draw background
        drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
                              IM_COL32(m_config.backgroundColor.x * 255,
                                      m_config.backgroundColor.y * 255,
                                      m_config.backgroundColor.z * 255,
                                      m_config.backgroundColor.w * 255));
        
        if (m_config.showGrid) {
            // Draw the grid with proper spacing
            float worldSpacing = m_config.gridSpacing;
            
            // Calculate where to start drawing grid lines
            float startX = std::ceil(m_viewMinX / worldSpacing) * worldSpacing;
            float startY = std::ceil(m_viewMinY / worldSpacing) * worldSpacing;
            
            // Draw vertical grid lines
            for (float x = startX; x <= m_viewMaxX; x += worldSpacing) {
                // Skip the axis line which will be drawn separately
                if (std::abs(x) < 0.001f) continue;
                
                float screenX = canvasPos.x + (x - m_viewMinX) * scaleX;
                drawList->AddLine(
                    ImVec2(screenX, canvasPos.y),
                    ImVec2(screenX, canvasPos.y + canvasSize.y),
                    IM_COL32(m_config.gridColor.x * 255,
                            m_config.gridColor.y * 255,
                            m_config.gridColor.z * 255,
                            m_config.gridColor.w * 255),
                    m_config.lineThickness * 0.5f
                );
                
                // Draw coordinate label if it's a major grid line
                if (std::fmod(std::abs(x), worldSpacing * 2.0f) < 0.001f) {
                    char label[32];
                    snprintf(label, sizeof(label), "%.1f", x);
                    float labelY = canvasPos.y + canvasSize.y - 20.0f; // Position near bottom
                    
                    // Keep the labelY constant, no need for axisX
                    drawList->AddText(
                        ImVec2(screenX - 10.0f, labelY),
                        IM_COL32(m_config.axisColor.x * 255,
                                m_config.axisColor.y * 255,
                                m_config.axisColor.z * 255,
                                m_config.axisColor.w * 255),
                        label
                    );
                }
            }
            
            // Draw horizontal grid lines
            for (float y = startY; y <= m_viewMaxY; y += worldSpacing) {
                // Skip the axis line which will be drawn separately
                if (std::abs(y) < 0.001f) continue;
                
                float screenY = canvasPos.y + canvasSize.y - (y - m_viewMinY) * scaleY;
                drawList->AddLine(
                    ImVec2(canvasPos.x, screenY),
                    ImVec2(canvasPos.x + canvasSize.x, screenY),
                    IM_COL32(m_config.gridColor.x * 255,
                            m_config.gridColor.y * 255,
                            m_config.gridColor.z * 255,
                            m_config.gridColor.w * 255),
                    m_config.lineThickness * 0.5f
                );
                
                // Draw coordinate label if it's a major grid line
                if (std::fmod(std::abs(y), worldSpacing * 2.0f) < 0.001f) {
                    char label[32];
                    snprintf(label, sizeof(label), "%.1f", y);
                    float labelX = canvasPos.x + 5.0f; // Position near left edge
                    
                    // Keep the labelX constant, no need for axisY
                    drawList->AddText(
                        ImVec2(labelX, screenY - 10.0f),
                        IM_COL32(m_config.axisColor.x * 255,
                                m_config.axisColor.y * 255,
                                m_config.axisColor.z * 255,
                                m_config.axisColor.w * 255),
                        label
                    );
                }
            }
            
            // Draw X and Y axes with thicker lines
            
            // Draw X axis (y = 0) if within view
            if (m_viewMinY <= 0 && m_viewMaxY >= 0) {
                float yZero = canvasPos.y + canvasSize.y - (0 - m_viewMinY) * scaleY;
                drawList->AddLine(
                    ImVec2(canvasPos.x, yZero),
                    ImVec2(canvasPos.x + canvasSize.x, yZero),
                    IM_COL32(m_config.axisColor.x * 255,
                            m_config.axisColor.y * 255,
                            m_config.axisColor.z * 255,
                            m_config.axisColor.w * 255),
                    m_config.lineThickness * 1.5f  // Make axis lines thicker
                );
            }
            
            // Draw Y axis (x = 0) if within view
            if (m_viewMinX <= 0 && m_viewMaxX >= 0) {
                float xZero = canvasPos.x + (0 - m_viewMinX) * scaleX;
                drawList->AddLine(
                    ImVec2(xZero, canvasPos.y),
                    ImVec2(xZero, canvasPos.y + canvasSize.y),
                    IM_COL32(m_config.axisColor.x * 255,
                            m_config.axisColor.y * 255,
                            m_config.axisColor.z * 255,
                            m_config.axisColor.w * 255),
                    m_config.lineThickness * 1.5f  // Make axis lines thicker
                );
            }
        }
        
        // Draw multiple equation points if available
        if (!m_equationPoints.empty()) {
            // Define equation colors - cycle through these for different equations
            const int numColors = 5;
            ImU32 colors[numColors] = {
                IM_COL32(0, 204, 51, 255),   // Green
                IM_COL32(51, 153, 255, 255), // Blue
                IM_COL32(255, 51, 51, 255),  // Red
                IM_COL32(255, 153, 51, 255), // Orange
                IM_COL32(153, 51, 255, 255)  // Purple
            };
            
            // Log points count for debugging
            if (debugCounter % 100 == 0) {
                std::cout << "Multiple equations to plot: " << m_equationPoints.size() << std::endl;
            }
            
            // Draw each equation's points with a different color
            for (size_t eq = 0; eq < m_equationPoints.size(); ++eq) {
                const auto& points = m_equationPoints[eq];
                ImU32 color = colors[eq % numColors];
                
                // Draw connecting lines between points for this equation
                for (size_t i = 1; i < points.size(); ++i) {
                    // Properly transform from world to screen coordinates
                    float x1 = canvasPos.x + (points[i-1].x - m_viewMinX) * scaleX;
                    float y1 = canvasPos.y + canvasSize.y - (points[i-1].y - m_viewMinY) * scaleY;
                    float x2 = canvasPos.x + (points[i].x - m_viewMinX) * scaleX;
                    float y2 = canvasPos.y + canvasSize.y - (points[i].y - m_viewMinY) * scaleY;
                    
                    // Only draw if at least one point is within view
                    if ((x1 >= canvasPos.x && x1 <= canvasPos.x + canvasSize.x) ||
                        (x2 >= canvasPos.x && x2 <= canvasPos.x + canvasSize.x)) {
                        if ((y1 >= canvasPos.y && y1 <= canvasPos.y + canvasSize.y) ||
                            (y2 >= canvasPos.y && y2 <= canvasPos.y + canvasSize.y)) {
                            drawList->AddLine(
                                ImVec2(x1, y1),
                                ImVec2(x2, y2),
                                color,
                                m_config.lineThickness
                            );
                        }
                    }
                }
            }
        }
        // Only use legacy single equation points if we don't have multi-equation points
        else if (!m_points.empty()) {
            // Log points count for debugging
            if (debugCounter % 100 == 0) {
                std::cout << "Points to plot: " << m_points.size() << std::endl;
            }
            
            // Draw connecting lines between points
            for (size_t i = 1; i < m_points.size(); ++i) {
                // Properly transform from world to screen coordinates
                float x1 = canvasPos.x + (m_points[i-1].x - m_viewMinX) * scaleX;
                float y1 = canvasPos.y + canvasSize.y - (m_points[i-1].y - m_viewMinY) * scaleY;
                float x2 = canvasPos.x + (m_points[i].x - m_viewMinX) * scaleX;
                float y2 = canvasPos.y + canvasSize.y - (m_points[i].y - m_viewMinY) * scaleY;
                
                // Only draw if at least one point is within view
                if ((x1 >= canvasPos.x && x1 <= canvasPos.x + canvasSize.x) ||
                    (x2 >= canvasPos.x && x2 <= canvasPos.x + canvasSize.x)) {
                    if ((y1 >= canvasPos.y && y1 <= canvasPos.y + canvasSize.y) ||
                        (y2 >= canvasPos.y && y2 <= canvasPos.y + canvasSize.y)) {
                            drawList->AddLine(
                                ImVec2(x1, y1),
                                ImVec2(x2, y2),
                                IM_COL32(m_config.graphColor.x * 255,
                                    m_config.graphColor.y * 255,
                                    m_config.graphColor.z * 255,
                                    m_config.graphColor.w * 255),
                                m_config.lineThickness
                            );
                    }
                }
            }
        } else {
            // Draw message if no points
            if (debugCounter % 100 == 0) {
                std::cout << "No points to plot" << std::endl;
            }
            
            ImVec2 msgPos = ImVec2(canvasPos.x + canvasSize.x * 0.5f - 60, canvasPos.y + canvasSize.y * 0.5f - 10);
            drawList->AddText(msgPos, IM_COL32(255, 255, 255, 255), "No data to display");
        }
        
        // Display all active equations instead of just the latest one
        // Position it below the window title bar to avoid overlap
        if (!m_equations.empty()) {
            ImGui::SetCursorPos(ImVec2(10, 30)); // Position below the title bar
            
            // Create a vertical list of all equations with their respective colors
            const int numColors = 5;
            ImU32 colors[numColors] = {
                IM_COL32(0, 204, 51, 255),   // Green
                IM_COL32(51, 153, 255, 255), // Blue
                IM_COL32(255, 51, 51, 255),  // Red
                IM_COL32(255, 153, 51, 255), // Orange
                IM_COL32(153, 51, 255, 255)  // Purple
            };
            
            for (size_t i = 0; i < m_equations.size(); ++i) {
                ImVec4 color = ImGui::ColorConvertU32ToFloat4(colors[i % numColors]);
                ImGui::TextColored(color, "%s", m_equations[i].c_str());
            }
        }
        
        // Display viewport coordinates
        ImGui::SetCursorPos(ImVec2(10, ImGui::GetWindowHeight() - 30));
        ImGui::Text("View: X[%.1f, %.1f] Y[%.1f, %.1f]", 
                    m_viewMinX, m_viewMaxX, m_viewMinY, m_viewMaxY);
        
        // Continue handling input
        HandleInput();
    }
    ImGui::End();
}

void GraphPanel::SetPoints(const std::vector<GraphPoint>& points) {
    m_points = points;
}

void GraphPanel::SetMultipleEquationPoints(const std::vector<std::vector<GraphPoint>>& equationPoints) {
    m_equationPoints = equationPoints;
    
    // No longer flattening points to avoid creating the false third line
    // This prevents the issue where points from different equations are connected
}

void GraphPanel::SetViewCallback(std::function<void(float, float, float, float)> callback) {
    m_viewCallback = std::move(callback);
}

void GraphPanel::SetEquation(const std::string& equation) {
    // Add this equation to our list if it's not already there
    bool found = false;
    for (const auto& eq : m_equations) {
        if (eq == equation) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        m_equations.push_back(equation);
    }
}

void GraphPanel::SetConfig(const GraphConfig& config) {
    m_config = config;
}

void GraphPanel::DrawGraph() {
    // Legacy method, now incorporated into Render()
}

void GraphPanel::HandleInput() {
    if (ImGui::IsWindowHovered()) {
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        bool changed = false;
        
        // Pan with left mouse button
        if (ImGui::IsMouseDragging(0)) {
            float dx = ImGui::GetIO().MouseDelta.x;
            float dy = ImGui::GetIO().MouseDelta.y;
            
            // Apply pan sensitivity (fixed value)
            dx *= m_config.panSensitivity;
            dy *= m_config.panSensitivity;
            
            // Convert screen space delta to world space delta
            float worldDeltaX = dx * (m_viewMaxX - m_viewMinX) / canvasSize.x;
            float worldDeltaY = dy * (m_viewMaxY - m_viewMinY) / canvasSize.y;
            
            // Apply X/Y sensitivity adjustments (using fixed values)
            worldDeltaX *= m_config.xAxisScaling;  // Fixed to 1.0
            worldDeltaY *= m_config.yAxisScaling;  // Fixed to 0.01
            
            // Move in the opposite direction of mouse movement
            m_viewMinX -= worldDeltaX;
            m_viewMaxX -= worldDeltaX;
            m_viewMinY += worldDeltaY;
            m_viewMaxY += worldDeltaY;
            
            changed = true;
        }

        // Zoom with mouse wheel
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f) {
            // Apply zoom sensitivity (fixed value)
            wheel *= m_config.zoomSensitivity;
            
            // Get mouse position in screen space
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 canvasPos = ImGui::GetCursorScreenPos();
            
            // Convert mouse position to relative coordinates (0 to 1)
            float relX = (mousePos.x - canvasPos.x) / canvasSize.x;
            float relY = (mousePos.y - canvasPos.y) / canvasSize.y;
            
            // Clamp to ensure we're within the canvas
            relX = std::max(0.0f, std::min(relX, 1.0f));
            relY = std::max(0.0f, std::min(relY, 1.0f));
            
            // Convert relative coordinates to world space
            float worldX = m_viewMinX + relX * (m_viewMaxX - m_viewMinX);
            float worldY = m_viewMinY + (1.0f - relY) * (m_viewMaxY - m_viewMinY); // Account for inverted Y
            
            // Calculate zoom factor - positive wheel scrolls in, negative scrolls out
            float zoomFactor = wheel > 0 ? (1.0f - 0.1f * m_config.zoomSensitivity) : (1.0f + 0.1f * m_config.zoomSensitivity);
            
            // Calculate new dimensions using the square zoom
            float xRange = (m_viewMaxX - m_viewMinX) * zoomFactor;
            float yRange = (m_viewMaxY - m_viewMinY) * zoomFactor;
            
            // Calculate new boundaries maintaining aspect ratio
            m_viewMinX = worldX - relX * xRange;
            m_viewMaxX = m_viewMinX + xRange;
            m_viewMinY = worldY - (1.0f - relY) * yRange;
            m_viewMaxY = m_viewMinY + yRange;
            
            changed = true;
        }

        // Reset view with right mouse button
        if (ImGui::IsMouseClicked(1)) {
            ResetView();
            changed = true;
        }
        
        if (changed) {
            UpdateView();
        }
    }
}

void GraphPanel::UpdateView() {
    if (m_viewCallback) {
        m_viewCallback(m_viewMinX, m_viewMaxX, m_viewMinY, m_viewMaxY);
    }
}

void GraphPanel::ResetView() {
    // Use the configurable default view dimensions with a single scaling value
    float halfSize = m_config.defaultViewScaling / 2.0f;
    
    // Make sure view is square
    m_viewMinX = -halfSize;
    m_viewMaxX = halfSize;
    m_viewMinY = -halfSize;
    m_viewMaxY = halfSize;
    
    // Update the view
    UpdateView();
}

void GraphPanel::RemoveEquation(const std::string& equation) {
    // Find and remove the equation from our list
    auto it = std::find(m_equations.begin(), m_equations.end(), equation);
    if (it != m_equations.end()) {
        m_equations.erase(it);
    }
}

} // namespace plot_genius 