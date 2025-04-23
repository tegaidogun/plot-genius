cmake_minimum_required(VERSION 3.10)
project(plot_genius)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenGL preference to GLVND
set(OpenGL_GL_PREFERENCE GLVND)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/glad/include
    ${OPENGL_INCLUDE_DIR}
    ${GLFW3_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/imgui)

# Create executable
add_executable(plot_genius src/main.cpp)

# Link libraries
target_link_libraries(plot_genius
    PRIVATE
    plot_genius_lib
    glad
    imgui
    ${OPENGL_LIBRARIES}
    glfw
    glm
    nlohmann_json::nlohmann_json
)

# Set compiler flags
if(MSVC)
    target_compile_options(plot_genius PRIVATE /W4)
else()
    target_compile_options(plot_genius PRIVATE -Wall -Wextra)
endif()

# Install rules
install(TARGETS plot_genius
    RUNTIME DESTINATION bin
)

# Add other libraries as needed

# Add other targets as needed

# Add other install rules as needed
 