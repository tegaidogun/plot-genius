#pragma once

#include <memory>
#include <string>

struct GLFWwindow;

namespace plot_genius {

class Window {
public:
    Window();
    ~Window();

    bool Initialize(const std::string& title, int width, int height);
    void Shutdown();

    bool ShouldClose() const;
    void SwapBuffers();
    void PollEvents();

    GLFWwindow* GetHandle() const { return m_window; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    GLFWwindow* m_window{nullptr};
    int m_width{0};
    int m_height{0};
};
} 