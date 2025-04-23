#pragma once

#include <memory>
#include <string>
#include "../ui/window.hpp"
#include "../core/logger.hpp"

namespace plot_genius {
namespace application {

class App {
public:
    static App& GetInstance();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    App();
    ~App();

    void HandleEvents();
    void Update();
    void Render();

    bool m_running;
    double m_fps;
    std::unique_ptr<Window> m_window;
};

} // namespace application
} // namespace plot_genius 