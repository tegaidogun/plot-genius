#include "application/app.hpp"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    try {
        auto& app = plot_genius::application::App::GetInstance();
        
        if (!app.Initialize()) {
            std::cerr << "Failed to initialize application" << std::endl;
            return 1;
        }
        
        app.Run();
        app.Shutdown();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 