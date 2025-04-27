/**
 * Plot Genius Application Entry Point
 * 
 * This file contains the main function that serves as the entry point for the Plot Genius application.
 * It initializes the application, runs the main loop, and handles shutdown, with basic exception handling.
 */

#include "application/app.hpp"
#include <iostream>

/**
 * Main application entry point
 * 
 * @param argc Command line argument count (unused)
 * @param argv Command line argument values (unused)
 * @return Exit status code (0 for success, 1 for failure)
 */
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