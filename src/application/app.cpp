/**
 * Implementation of the App class
 * 
 * Contains the implementation of the application lifecycle management including 
 * initialization, main loop execution, and shutdown procedures.
 */

#include "app.hpp"
#include <GLFW/glfw3.h>

namespace plot_genius {
namespace application {

App& App::GetInstance() {
    static App instance;
    return instance;
}

App::App()
    : m_running(false)
    , m_fps(0.0)
    , m_window(nullptr) {}

App::~App() {
    Shutdown();
}

bool App::Initialize() {
    core::Logger::GetInstance().Log(core::LogLevel::Info, "Initializing application");

    m_window = std::make_unique<Window>();
    if (!m_window->Initialize()) {
        core::Logger::GetInstance().Log(core::LogLevel::Error, "Failed to initialize window");
        return false;
    }

    m_running = true;
    return true;
}

void App::Run() {
    core::Logger::GetInstance().Log(core::LogLevel::Info, "Starting main loop");

    double lastTime = glfwGetTime();
    double frameTime = 0.0;
    int frameCount = 0;

    while (m_running && !m_window->ShouldClose()) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        frameTime += deltaTime;
        frameCount++;

        // Calculate FPS once per second
        if (frameTime >= 1.0) {
            m_fps = frameCount / frameTime;
            frameTime = 0.0;
            frameCount = 0;
        }

        HandleEvents();
        Update();
        Render();
    }
}

void App::Shutdown() {
    core::Logger::GetInstance().Log(core::LogLevel::Info, "Shutting down application");

    if (m_window) {
        m_window->Shutdown();
        m_window.reset();
    }

    glfwTerminate();
    m_running = false;
}

void App::HandleEvents() {
    // Handle any input events here
}

void App::Update() {
    m_window->SetTitle("Plot Genius - FPS: " + std::to_string(static_cast<int>(m_fps)));
}

void App::Render() {
    m_window->Render();
    m_window->SwapBuffers();
}

} // namespace application
} // namespace plot_genius 