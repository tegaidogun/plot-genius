# Add source files
set(SOURCES
    core/logger.cpp
    config/config.cpp
    equation/parser.cpp
    graph/graph.cpp
    rendering/renderer.cpp
    ui/window.cpp
    ui/graph_panel.cpp
    ui/equation_panel.cpp
    ui/config_panel.cpp
    application/app.cpp
)

# Add header files
set(HEADERS
    core/logger.hpp
    config/config.hpp
    equation/parser.hpp
    graph/graph.hpp
    rendering/renderer.hpp
    ui/window.hpp
    ui/graph_panel.hpp
    ui/equation_panel.hpp
    ui/config_panel.hpp
    application/app.hpp
)

# Create library
add_library(plot_genius_lib STATIC ${SOURCES} ${HEADERS})

# Set include directories
target_include_directories(plot_genius_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/glm
)

# Link dependencies
target_link_libraries(plot_genius_lib PUBLIC
    OpenGL::GL
    imgui
    glad
    glfw
) 