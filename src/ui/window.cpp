#include "window.hpp"
#include "../core/logger.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace plot_genius {

Window::Window()
    : m_window(nullptr)
    , m_equation("")
    , m_xMin(-5.0)
    , m_xMax(5.0)
    , m_yMin(-5.0)
    , m_yMax(5.0)
    , m_graph(std::make_unique<Graph>()) {}

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(1280, 720, "Plot Genius", nullptr, nullptr);
    if (!m_window) {
        core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

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

    core::Logger::GetInstance().Log(core::LogLevel::Info, "Window initialized successfully");
    return true;
}

void Window::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create main window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(display_w), static_cast<float>(display_h)));
    ImGui::Begin("Plot Genius", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    RenderEquationInput();
    RenderControls();
    RenderGraph();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::RenderEquationInput() {
    ImGui::Text("Enter equation:");
    char buffer[256];
    strncpy(buffer, m_equation.c_str(), sizeof(buffer));
    if (ImGui::InputText("##equation", buffer, sizeof(buffer))) {
        m_equation = buffer;
        try {
            if (m_graph->SetEquation(m_equation)) {
                core::Logger::GetInstance().Log(core::LogLevel::Info, "Equation updated: " + m_equation);
            }
        } catch (const std::exception& e) {
            core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to parse equation: " + std::string(e.what()));
        }
    }
}

void Window::RenderControls() {
    ImGui::Separator();
    ImGui::Text("Graph Controls");

    bool rangeChanged = false;
    rangeChanged |= ImGui::SliderFloat("X Min", &m_xMin, -10.0f, m_xMax - 0.1f);
    rangeChanged |= ImGui::SliderFloat("X Max", &m_xMax, m_xMin + 0.1f, 10.0f);
    rangeChanged |= ImGui::SliderFloat("Y Min", &m_yMin, -10.0f, m_yMax - 0.1f);
    rangeChanged |= ImGui::SliderFloat("Y Max", &m_yMax, m_yMin + 0.1f, 10.0f);

    if (rangeChanged) {
        core::Logger::GetInstance().Log(core::LogLevel::Info, "Graph range updated");
    }
}

void Window::RenderGraph() {
    ImGui::Separator();
    ImGui::Text("Graph");

    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Draw background
    draw_list->AddRectFilled(canvas_pos, 
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
        IM_COL32(50, 50, 50, 255));

    // Draw grid
    float grid_step = canvas_size.x / 20.0f;
    for (float x = 0; x <= canvas_size.x; x += grid_step) {
        draw_list->AddLine(
            ImVec2(canvas_pos.x + x, canvas_pos.y),
            ImVec2(canvas_pos.x + x, canvas_pos.y + canvas_size.y),
            IM_COL32(100, 100, 100, 40));
    }
    for (float y = 0; y <= canvas_size.y; y += grid_step) {
        draw_list->AddLine(
            ImVec2(canvas_pos.x, canvas_pos.y + y),
            ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + y),
            IM_COL32(100, 100, 100, 40));
    }

    // Draw axes
    draw_list->AddLine(
        ImVec2(canvas_pos.x, canvas_pos.y + canvas_size.y / 2),
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y / 2),
        IM_COL32(255, 255, 255, 255));
    draw_list->AddLine(
        ImVec2(canvas_pos.x + canvas_size.x / 2, canvas_pos.y),
        ImVec2(canvas_pos.x + canvas_size.x / 2, canvas_pos.y + canvas_size.y),
        IM_COL32(255, 255, 255, 255));

    // Draw graph
    if (!m_equation.empty()) {
        std::vector<ImVec2> points;
        const int num_points = 200;
        for (int i = 0; i < num_points; ++i) {
            float t = static_cast<float>(i) / (num_points - 1);
            float x = m_xMin + t * (m_xMax - m_xMin);
            try {
                double y = m_graph->Evaluate(x);
                if (y >= m_yMin && y <= m_yMax) {
                    float screen_x = canvas_pos.x + (x - m_xMin) / (m_xMax - m_xMin) * canvas_size.x;
                    float screen_y = canvas_pos.y + (1.0f - (y - m_yMin) / (m_yMax - m_yMin)) * canvas_size.y;
                    points.push_back(ImVec2(screen_x, screen_y));
                }
            } catch (const std::exception&) {
                // Skip invalid points
            }
        }

        if (points.size() > 1) {
            for (size_t i = 1; i < points.size(); ++i) {
                draw_list->AddLine(points[i - 1], points[i], IM_COL32(0, 255, 0, 255), 2.0f);
            }
        }
    }

    ImGui::SetCursorScreenPos(ImVec2(canvas_pos.x, canvas_pos.y + canvas_size.y + 10));
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

std::vector<Point> Window::GetGraphPoints() const {
    return m_graph->GeneratePoints(m_xMin, m_xMax);
}

} // namespace plot_genius