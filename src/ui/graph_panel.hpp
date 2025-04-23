#pragma once

#include <vector>
#include <functional>

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

private:
    std::vector<GraphPoint> m_points;
    float m_viewMinX{-10.0f};
    float m_viewMaxX{10.0f};
    float m_viewMinY{-10.0f};
    float m_viewMaxY{10.0f};
    std::function<void(float, float, float, float)> m_viewCallback;

    void DrawGraph();
    void HandleInput();
    void UpdateView();
}; 