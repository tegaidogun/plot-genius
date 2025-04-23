#pragma once

#include <vector>
#include <functional>
#include <string>
#include "config_panel.hpp"

namespace plot_genius {

struct GraphPoint {
    float x;
    float y;
};

class GraphPanel {
public:
    GraphPanel();
    ~GraphPanel() = default;

    void Render();
    void SetPoints(const std::vector<GraphPoint>& points);
    void SetViewCallback(std::function<void(float, float, float, float)> callback);
    void SetEquation(const std::string& equation);
    void SetConfig(const GraphConfig& config);
    void ResetView();
    
    // Viewport getters
    float GetViewMinX() const { return m_viewMinX; }
    float GetViewMaxX() const { return m_viewMaxX; }
    float GetViewMinY() const { return m_viewMinY; }
    float GetViewMaxY() const { return m_viewMaxY; }

private:
    std::vector<GraphPoint> m_points;
    std::string m_equation;
    GraphConfig m_config;
    float m_viewMinX{-10.0f};
    float m_viewMaxX{10.0f};
    float m_viewMinY{-10.0f};
    float m_viewMaxY{10.0f};
    std::function<void(float, float, float, float)> m_viewCallback;

    void DrawGraph();
    void HandleInput();
    void UpdateView();
};

} // namespace plot_genius 