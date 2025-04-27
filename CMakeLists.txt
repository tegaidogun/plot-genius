cmake_minimum_required(VERSION 3.10)
project(plot_genius VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenGL preference to GLVND
set(OpenGL_GL_PREFERENCE GLVND)

# Only find OpenGL from system (required)
find_package(OpenGL REQUIRED)

# Options
option(USE_SYSTEM_PACKAGES "Use system packages instead of bundled libraries" OFF)
option(WITH_WAYLAND "Enable Wayland support" ON)
option(WITHOUT_X11 "Disable X11 support" ON)

# Set various defines needed to compile
if(WITH_WAYLAND)
    message(STATUS "Including Wayland support")
    add_definitions(-DPLOT_GENIUS_WITH_WAYLAND)
else()
    message(STATUS "Excluding Wayland support")
endif()

if(WITHOUT_X11)
    message(STATUS "Excluding X11 support")
    add_definitions(-DPLOT_GENIUS_WITHOUT_X11)
else()
    message(STATUS "Including X11 support")
endif()

# Include GLFW Wayland and X11 support based on options
if(WITH_WAYLAND)
    set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
else()
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
endif()

if(WITHOUT_X11)
    set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
else()
    set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
endif()

# Add other GLFW options
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/glad/include
    ${CMAKE_SOURCE_DIR}/thirdparty/glm
    ${OPENGL_INCLUDE_DIR}
)

# Decide whether to use system packages or bundled libraries
if(USE_SYSTEM_PACKAGES)
    message(STATUS "Using system packages for dependencies")
    find_package(glfw3 REQUIRED)
    find_package(imgui REQUIRED)
    find_package(glm REQUIRED)
    
    # Include directories
    include_directories(${GLFW_INCLUDE_DIRS})
    include_directories(${IMGUI_INCLUDE_DIRS})
    include_directories(${GLM_INCLUDE_DIRS})
    
    # If Wayland is enabled, find Wayland packages
    if(WITH_WAYLAND)
        find_package(Wayland REQUIRED)
        include_directories(${WAYLAND_INCLUDE_DIRS})
    endif()
else()
    message(STATUS "Using bundled libraries from thirdparty directory")
    # Add bundled libraries
    add_subdirectory(thirdparty/glad)
    add_subdirectory(thirdparty/glfw)
    add_subdirectory(thirdparty/imgui)
    
    # Include directories for bundled libraries
    include_directories(thirdparty/glad/include)
    include_directories(thirdparty/glfw/include)
    include_directories(thirdparty/imgui)
    include_directories(thirdparty/imgui/backends)
    include_directories(thirdparty/glm)
endif()

# Add source subdirectory
add_subdirectory(src)

# Create the executable
add_executable(plot_genius src/main.cpp)

# Link libraries
if(USE_SYSTEM_PACKAGES)
    target_link_libraries(plot_genius PRIVATE plot_genius_lib ${OPENGL_LIBRARIES} glfw imgui::imgui)
    
    # Link Wayland libraries if needed
    if(WITH_WAYLAND)
        target_link_libraries(plot_genius PRIVATE ${WAYLAND_LIBRARIES})
    endif()
else()
    target_link_libraries(plot_genius PRIVATE plot_genius_lib ${OPENGL_LIBRARIES} glad glfw imgui)
endif()

# Set compiler flags
if(MSVC)
    target_compile_options(plot_genius PRIVATE /W4 /WX)
else()
    target_compile_options(plot_genius PRIVATE -Wall -Wextra)
endif()

# Installation
install(TARGETS plot_genius
    RUNTIME DESTINATION bin
)
 