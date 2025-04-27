#pragma once

#include <memory>
#include <string>
#include "../ui/window.hpp"
#include "../core/logger.hpp"

namespace plot_genius {
namespace application {

/**
 * Main application class managing application lifecycle
 */
class App {
public:
    /**
     * Returns the singleton instance of the application
     */
    static App& GetInstance();

    /**
     * Initializes application resources and components
     * @return Success status of initialization
     */
    bool Initialize();
    
    /**
     * Executes the main application loop
     */
    void Run();
    
    /**
     * Cleans up resources and performs application shutdown
     */
    void Shutdown();

private:
    /**
     * Private constructor for singleton pattern
     */
    App();
    
    /**
     * Private destructor for singleton pattern
     */
    ~App();

    /**
     * Processes pending application events
     */
    void HandleEvents();
    
    /**
     * Updates application state
     */
    void Update();
    
    /**
     * Renders the current frame
     */
    void Render();

    bool m_running;
    double m_fps;
    std::unique_ptr<Window> m_window;
};

} // namespace application
} // namespace plot_genius 