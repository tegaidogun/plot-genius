/**
 * Window Management Header
 * 
 * Defines the Window class that encapsulates GLFW window functionality and provides
 * an abstraction layer for window-related operations.
 */

#pragma once

#include <memory>
#include <string>

struct GLFWwindow;

namespace plot_genius {

/**
 * Window class providing an abstraction over GLFW window functionality
 */
class Window {
public:
    /**
     * Default constructor
     */
    Window();
    
    /**
     * Destructor that ensures window resources are properly released
     */
    ~Window();

    /**
     * Initializes a window with the specified parameters
     * 
     * @param title The window title
     * @param width The window width in pixels
     * @param height The window height in pixels
     * @return Whether initialization was successful
     */
    bool Initialize(const std::string& title, int width, int height);
    
    /**
     * Releases window resources
     */
    void Shutdown();

    /**
     * Checks if the window has been requested to close
     * 
     * @return True if the window should close, false otherwise
     */
    bool ShouldClose() const;
    
    /**
     * Swaps the front and back buffers
     */
    void SwapBuffers();
    
    /**
     * Processes pending window events
     */
    void PollEvents();

    /**
     * Gets the underlying GLFW window handle
     * 
     * @return Pointer to the GLFW window
     */
    GLFWwindow* GetHandle() const { return m_window; }
    
    /**
     * Gets the current window width
     * 
     * @return Window width in pixels
     */
    int GetWidth() const { return m_width; }
    
    /**
     * Gets the current window height
     * 
     * @return Window height in pixels
     */
    int GetHeight() const { return m_height; }

private:
    GLFWwindow* m_window{nullptr};
    int m_width{0};
    int m_height{0};
};
} 