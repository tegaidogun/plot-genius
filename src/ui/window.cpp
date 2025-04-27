#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

#include "window.hpp"
#include "../core/logger.hpp"

namespace plot_genius {

Window::Window()
    : m_window(nullptr)
    , m_graphPanel(std::make_unique<GraphPanel>())
    , m_equationPanel(std::make_unique<EquationPanel>())
    , m_configPanel(std::make_unique<ConfigPanel>()) {}

Window::~Window() {
    Shutdown();
}

void Window::Shutdown() {
    if (m_window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool Window::Initialize() {
    core::Logger::GetInstance().Log(core::LogLevel::Info, "Initializing window...");

    if (!glfwInit()) {
        core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to initialize GLFW");
        return false;
    }

    // Set OpenGL context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Essential window hints for proper window behavior with decorations
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    
    // Create window with default decorations
    m_window = glfwCreateWindow(1280, 720, "Plot Genius", nullptr, nullptr);
    if (!m_window) {
        core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to initialize GLAD");
        return false;
    }

    // Set up OpenGL viewport
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true)) {
        core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to initialize ImGui GLFW implementation");
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to initialize ImGui OpenGL3 implementation");
        return false;
    }

    // Set up initial graph config
    GraphConfig defaultConfig;
    defaultConfig.showGrid = true;
    defaultConfig.gridSpacing = 1.0f;
    defaultConfig.lineThickness = 2.0f;
    defaultConfig.defaultViewScaling = 20.0f;  // Single value for both dimensions
    defaultConfig.xAxisScaling = 1.0f;       // Fixed value for X sensitivity
    defaultConfig.yAxisScaling = 0.01f;      // Fixed value for Y sensitivity
    m_graphPanel->SetConfig(defaultConfig);

    // Set up panel callbacks
    m_equationPanel->SetEquationCallback([this](const std::string& equation) {
        // Add or update this equation
        UpdateGraphPoints(equation);
    });
    
    // Set up equation removal callback
    m_equationPanel->SetRemoveCallback([this](int id) {
        // Remove equation
        RemoveEquation(id);
    });

    m_graphPanel->SetViewCallback([this](float minX, float maxX, float minY, float maxY) {
        // Regenerate points for all active equations with the new view
        UpdateActiveGraphPoints();
    });
    
    // Set up config callback
    m_configPanel->SetConfigCallback([this](const GraphConfig& config) {
        m_graphPanel->SetConfig(config);
    });

    // No default equation - let user add one
    
    core::Logger::GetInstance().Log(core::LogLevel::Info, "Window initialized successfully");
    return true;
}

void Window::Render() {
    // Clear the framebuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Set ImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().FrameRounding = 4.0f;
    ImGui::GetStyle().GrabRounding = 4.0f;
    
    // Get display size
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    
    // Calculate panel sizes
    // Sidebar widths (equal on both sides)
    const float minSidebarWidth = 200.0f;
    const float maxGraphWidth = displaySize.x * 0.75f; // Max 75% for graph
    const float minGraphWidth = displaySize.x * 0.5f;  // Min 50% for graph
    
    // Calculate graph size (should be square)
    float graphSize = std::min(displaySize.y, maxGraphWidth); // Start with max possible square
    graphSize = std::max(graphSize, minGraphWidth); // Ensure minimum width
    
    // Calculate sidebar widths based on remaining space
    float remainingWidth = displaySize.x - graphSize;
    float sidebarWidth = remainingWidth / 2.0f;
    
    // Ensure minimum sidebar width
    if (sidebarWidth < minSidebarWidth) {
        sidebarWidth = minSidebarWidth;
        // Recalculate graph width
        graphSize = displaySize.x - (sidebarWidth * 2.0f);
    }
    
    // -------------------- LEFT PANEL (EQUATIONS) --------------------
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(sidebarWidth, displaySize.y));
    ImGuiWindowFlags leftPanelFlags = ImGuiWindowFlags_NoCollapse | 
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoResize;
    
    if (ImGui::Begin("Equations", nullptr, leftPanelFlags)) {
        m_equationPanel->Render();
    }
    ImGui::End();
    
    // -------------------- CENTER PANEL (GRAPH) --------------------
    ImGui::SetNextWindowPos(ImVec2(sidebarWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(graphSize, displaySize.y));
    ImGuiWindowFlags graphFlags = ImGuiWindowFlags_NoCollapse | 
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize;
    
    if (ImGui::Begin("Graph", nullptr, graphFlags)) {
        m_graphPanel->Render();
    }
    ImGui::End();
    
    // -------------------- RIGHT PANEL (CONFIG) --------------------
    ImGui::SetNextWindowPos(ImVec2(sidebarWidth + graphSize, 0));
    ImGui::SetNextWindowSize(ImVec2(sidebarWidth, displaySize.y));
    ImGuiWindowFlags rightPanelFlags = ImGuiWindowFlags_NoCollapse | 
                                      ImGuiWindowFlags_NoMove |
                                      ImGuiWindowFlags_NoResize;
    
    // Config panel
    if (ImGui::Begin("Configuration", nullptr, rightPanelFlags)) {
        m_configPanel->Render();
    }
    ImGui::End();
    
    // Check if we should reset the graph view
    if (m_configPanel->ShouldResetGraphView()) {
        m_graphPanel->ResetView();
        core::Logger::GetInstance().Log(core::LogLevel::Info, "Graph view reset");
        m_configPanel->ClearResetFlag();
    }
    
    // Process rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Log frame rendering (useful for debugging)
    static int frameCount = 0;
    if (frameCount++ % 300 == 0) {
        core::Logger::GetInstance().Log(core::LogLevel::Debug, 
            "Layout: Graph=" + std::to_string(graphSize) + 
            "x" + std::to_string(displaySize.y) +
            ", Sidebars=" + std::to_string(sidebarWidth));
    }
}

void Window::UpdateGraphPoints(const std::string& equation) {
    if (equation.empty()) return;
    
    try {
        // Find or create an entry for this equation
        int id = -1;
        for (const auto& pair : m_equations) {
            if (pair.second.equation == equation) {
                id = pair.first;
                break;
            }
        }
        
        // If new equation, assign a new ID
        if (id == -1) {
            id = m_equations.empty() ? 0 : m_equations.rbegin()->first + 1;
            m_equations[id] = EquationGraph{};
            m_equations[id].equation = equation;
            m_equations[id].graph = std::make_unique<Graph>();
            m_equations[id].isActive = true;
        }
        
        // Set equation and generate points
        EquationGraph& eqGraph = m_equations[id];
        if (eqGraph.graph->SetEquation(equation)) {
            // Generate graph points
            auto points = eqGraph.graph->GeneratePoints(
                m_graphPanel->GetViewMinX(), 
                m_graphPanel->GetViewMaxX(), 
                200
            );
            
            // Convert to GraphPoint format
            eqGraph.points.clear();
            for (const auto& point : points) {
                eqGraph.points.push_back({static_cast<float>(point.x), static_cast<float>(point.y)});
            }
            
            // Collect points from all active equations
            std::vector<std::vector<GraphPoint>> allEquationPoints;
            for (const auto& pair : m_equations) {
                if (pair.second.isActive) {
                    allEquationPoints.push_back(pair.second.points);
                }
            }
            
            // Update the graph panel with the points from all active equations
            m_graphPanel->SetMultipleEquationPoints(allEquationPoints);
            m_graphPanel->SetEquation(equation); // Display the most recently added equation
            
            // Log success
            std::string message = "Generated " + std::to_string(eqGraph.points.size()) + 
                                  " points for equation: " + equation;
            core::Logger::GetInstance().Log(core::LogLevel::Info, message);
        } else {
            core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to parse equation: " + equation);
        }
    } catch (const std::exception& e) {
        std::string message = "Failed to generate graph: ";
        message += e.what();
        core::Logger::GetInstance().Log(core::LogLevel::Error, message);
    }
}

void Window::UpdateActiveGraphPoints() {
    // Regenerate points for all active equations with the current view
    std::vector<std::vector<GraphPoint>> allEquationPoints;
    
    for (auto& pair : m_equations) {
        auto& eqGraph = pair.second;
        if (eqGraph.isActive) {
            // Generate points for this equation
            auto points = eqGraph.graph->GeneratePoints(
                m_graphPanel->GetViewMinX(), 
                m_graphPanel->GetViewMaxX(), 
                200
            );
            
            // Convert to GraphPoint format
            eqGraph.points.clear();
            for (const auto& point : points) {
                eqGraph.points.push_back({static_cast<float>(point.x), static_cast<float>(point.y)});
            }
            
            // Add points to collection
            allEquationPoints.push_back(eqGraph.points);
        }
    }
    
    // Update the graph panel with all active points
    m_graphPanel->SetMultipleEquationPoints(allEquationPoints);
}

void Window::RemoveEquation(int id) {
    // Find and remove the equation from our collection
    auto it = m_equations.find(id);
    if (it != m_equations.end()) {
        // Get the equation text before removing it
        std::string equationText = it->second.equation;
        
        // Remove from our collection
        m_equations.erase(it);
        
        // Update active graph points
        UpdateActiveGraphPoints();
        
        // Also remove from the graph panel's equation list
        m_graphPanel->RemoveEquation(equationText);
    }
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::SetTitle(const std::string& title) {
    glfwSetWindowTitle(m_window, title.c_str());
}

} // namespace plot_genius
