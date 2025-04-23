#include "graph_panel.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace plot_genius {

GraphPanel::GraphPanel() = default;

void GraphPanel::Render() {
    if (ImGui::Begin("Graph")) {
        HandleInput();
        DrawGraph();
    }
    ImGui::End();
}

void GraphPanel::SetPoints(const std::vector<GraphPoint>& points) {
    m_points = points;
}

void GraphPanel::SetViewCallback(std::function<void(float, float, float, float)> callback) {
    m_viewCallback = std::move(callback);
}

void GraphPanel::DrawGraph() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();

    // Draw background
    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255));

    // Draw axes
    float center_x = canvas_pos.x + canvas_size.x * 0.5f;
    float center_y = canvas_pos.y + canvas_size.y * 0.5f;
    draw_list->AddLine(ImVec2(canvas_pos.x, center_y), ImVec2(canvas_pos.x + canvas_size.x, center_y), IM_COL32(255, 255, 255, 255));
    draw_list->AddLine(ImVec2(center_x, canvas_pos.y), ImVec2(center_x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));

    // Draw points
    if (!m_points.empty()) {
        for (size_t i = 0; i < m_points.size() - 1; ++i) {
            float x1 = center_x + (m_points[i].x - m_viewMinX) / (m_viewMaxX - m_viewMinX) * canvas_size.x;
            float y1 = center_y - (m_points[i].y - m_viewMinY) / (m_viewMaxY - m_viewMinY) * canvas_size.y;
            float x2 = center_x + (m_points[i + 1].x - m_viewMinX) / (m_viewMaxX - m_viewMinX) * canvas_size.x;
            float y2 = center_y - (m_points[i + 1].y - m_viewMinY) / (m_viewMaxY - m_viewMinY) * canvas_size.y;
            draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 255, 0, 255));
        }
    }
}

void GraphPanel::HandleInput() {
    if (ImGui::IsWindowHovered()) {
        if (ImGui::IsMouseDragging(0)) {
            float dx = ImGui::GetIO().MouseDelta.x;
            float dy = ImGui::GetIO().MouseDelta.y;
            float scale_x = (m_viewMaxX - m_viewMinX) / ImGui::GetWindowWidth();
            float scale_y = (m_viewMaxY - m_viewMinY) / ImGui::GetWindowHeight();
            m_viewMinX -= dx * scale_x;
            m_viewMaxX -= dx * scale_x;
            m_viewMinY += dy * scale_y;
            m_viewMaxY += dy * scale_y;
            UpdateView();
        }

        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f) {
            float zoom_factor = wheel > 0 ? 0.9f : 1.1f;
            float center_x = (m_viewMinX + m_viewMaxX) * 0.5f;
            float center_y = (m_viewMinY + m_viewMaxY) * 0.5f;
            float width = (m_viewMaxX - m_viewMinX) * zoom_factor;
            float height = (m_viewMaxY - m_viewMinY) * zoom_factor;
            m_viewMinX = center_x - width * 0.5f;
            m_viewMaxX = center_x + width * 0.5f;
            m_viewMinY = center_y - height * 0.5f;
            m_viewMaxY = center_y + height * 0.5f;
            UpdateView();
        }
    }
}

void GraphPanel::UpdateView() {
    if (m_viewCallback) {
        m_viewCallback(m_viewMinX, m_viewMaxX, m_viewMinY, m_viewMaxY);
    }
}

} // namespace plot_genius 