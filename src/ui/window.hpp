#pragma once

#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include "../graph/graph.hpp"
#include "../core/logger.hpp"

namespace plot_genius {

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
    void RenderEquationInput();
    void RenderControls();
    void RenderGraph();

    GLFWwindow* m_window;  // Store window pointer
    std::string m_equation;
    bool m_showGrid;
    float m_xMin;
    float m_xMax;
    float m_yMin;
    float m_yMax;
    std::unique_ptr<Graph> m_graph;
    bool m_shouldClose;
};

} // namespace plot_genius 