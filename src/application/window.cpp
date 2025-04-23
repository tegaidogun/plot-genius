#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace plot_genius {

Window::Window() = default;

Window::~Window() {
    Shutdown();
}

bool Window::Initialize(const std::string& title, int width, int height) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    m_width = width;
    m_height = height;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    return true;
}

void Window::Shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

} // namespace plot_genius 