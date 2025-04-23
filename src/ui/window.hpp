#pragma once

#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include <map>
#include "../graph/graph.hpp"
#include "../core/logger.hpp"
#include "graph_panel.hpp"
#include "equation_panel.hpp"
#include "config_panel.hpp"

namespace plot_genius {

struct EquationGraph {
    std::string equation;
    std::unique_ptr<Graph> graph;
    std::vector<GraphPoint> points;
    bool isActive{true};
};

class Window {
public:
    Window();
    ~Window();

    // Initialize the window and UI
    bool Initialize();

    // Shutdown the window
    void Shutdown();

    // Main render loop
    void Render();

    // Check if window should close
    bool ShouldClose() const;

    // Swap buffers
    void SwapBuffers();

    // Set window title
    void SetTitle(const std::string& title);

    // Get graph points
    std::vector<Point> GetGraphPoints() const;

private:
    void UpdateGraphPoints(const std::string& equation);
    void UpdateActiveGraphPoints();
    void RemoveEquation(int id);

    ::GLFWwindow* m_window;  // Store window pointer
    std::map<int, EquationGraph> m_equations;
    float m_xMin;
    float m_xMax;
    float m_yMin;
    float m_yMax;
    
    // UI Panels
    std::unique_ptr<GraphPanel> m_graphPanel;
    std::unique_ptr<EquationPanel> m_equationPanel;
    std::unique_ptr<ConfigPanel> m_configPanel;
    bool m_shouldClose;
};

} // namespace plot_genius 