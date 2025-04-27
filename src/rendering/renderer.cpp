#include <glad/glad.h>
#include "renderer.hpp"
#include <GLFW/glfw3.h>
#include "../core/logger.hpp"
#include <vector>
#include <cmath>
#include <stdexcept>

namespace plot_genius {
namespace rendering {

using core::Logger;

// OpenGL constants
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_LINES 0x0001
#define GL_LINE_STRIP 0x0003

// OpenGL function declarations
extern "C" {
    void glDeleteProgram(GLuint program);
    void glDeleteBuffers(GLsizei n, const GLuint* buffers);
    void glUseProgram(GLuint program);
    GLint glGetUniformLocation(GLuint program, const GLchar* name);
    void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void glBindVertexArray(GLuint array);
    void glBindBuffer(GLenum target, GLuint buffer);
    void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    void glEnableVertexAttribArray(GLuint index);
    void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
    void glDisableVertexAttribArray(GLuint index);
    GLuint glCreateShader(GLenum type);
    void glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
    void glCompileShader(GLuint shader);
    void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
    void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    GLuint glCreateProgram(void);
    void glAttachShader(GLuint program, GLuint shader);
    void glLinkProgram(GLuint program);
    void glGetProgramiv(GLuint program, GLenum pname, GLint* params);
    void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    void glGenVertexArrays(GLsizei n, GLuint* arrays);
    void glGenBuffers(GLsizei n, GLuint* buffers);
    void glDrawArrays(GLenum mode, GLint first, GLsizei count);
    void glClear(GLbitfield mask);
    void glFlush(void);
    void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
}

Renderer::Renderer() 
    : m_graphShader{0, 0, 0}
    , m_gridShader{0, 0, 0}
    , m_graphBuffer{0, 0}
    , m_gridBuffer{0, 0}
    , m_projection(1.0f)
    , m_view(1.0f)
    , m_gridVertexCount(0) {}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::GetInstance().Error("Failed to initialize GLAD");
        return false;
    }

    if (!SetupShaders()) {
        Logger::GetInstance().Error("Failed to setup shaders");
        return false;
    }

    CreateGraphBuffers();
    CreateGridBuffers();
    UpdateMatrices();

    return true;
}

void Renderer::Shutdown() {
    if (m_graphShader.id) {
        glDeleteProgram(m_graphShader.id);
        m_graphShader.id = 0;
    }
    if (m_gridShader.id) {
        glDeleteProgram(m_gridShader.id);
        m_gridShader.id = 0;
    }
    if (m_graphBuffer.vbo) {
        glDeleteBuffers(1, &m_graphBuffer.vbo);
        m_graphBuffer.vbo = 0;
    }
    if (m_gridBuffer.vbo) {
        glDeleteBuffers(1, &m_gridBuffer.vbo);
        m_gridBuffer.vbo = 0;
    }
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndFrame() {
    glFlush();
}

void Renderer::RenderGraph(const std::vector<Point>& points, 
                         double xMin, double xMax,
                         const config::Config::GraphSettings& settings) {
    if (points.empty()) return;

    glUseProgram(m_graphShader.id);
    
    // Update uniforms
    glUniformMatrix4fv(glGetUniformLocation(m_graphShader.id, "projection"), 1, GL_FALSE, &m_projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_graphShader.id, "view"), 1, GL_FALSE, &m_view[0][0]);

    // Convert points to vertices
    std::vector<float> vertices;
    vertices.reserve(points.size() * 2);
    for (const auto& point : points) {
        vertices.push_back(static_cast<float>(point.x));
        vertices.push_back(static_cast<float>(point.y));
    }

    // Update vertex data
    glBindVertexArray(m_graphBuffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_graphBuffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // Draw points
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(points.size()));
}

void Renderer::RenderGrid(double xMin, double xMax, double yMin, double yMax) {
    glUseProgram(m_gridShader.id);

    // Update uniforms
    glUniformMatrix4fv(glGetUniformLocation(m_gridShader.id, "projection"), 1, GL_FALSE, &m_projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_gridShader.id, "view"), 1, GL_FALSE, &m_view[0][0]);

    // Draw grid
    glBindVertexArray(m_gridBuffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer.vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glDrawArrays(GL_LINES, 0, m_gridVertexCount);

    // Cleanup
    glDisableVertexAttribArray(0);
    glUseProgram(0);
}

void Renderer::RenderAxes(double xMin, double xMax, double yMin, double yMax) {
    glUseProgram(m_gridShader.id);

    // Update uniforms
    glUniformMatrix4fv(glGetUniformLocation(m_gridShader.id, "projection"), 1, GL_FALSE, &m_projection[0][0]);

    // Draw axes
    glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer.vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glDrawArrays(GL_LINES, 0, 4); // 2 lines (x and y axes)

    // Cleanup
    glDisableVertexAttribArray(0);
    glUseProgram(0);
}

void Renderer::SetViewport(int width, int height) {
    glViewport(0, 0, width, height);
    UpdateMatrices();
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

bool Renderer::CompileShader(const char* source, unsigned int type, unsigned int& shader) {
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        Logger::GetInstance().Error("Shader compilation failed: {}", infoLog);
        return false;
    }
    return true;
}

bool Renderer::LinkProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int& program) {
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        Logger::GetInstance().Error("Shader program linking failed: {}", infoLog);
        return false;
    }
    return true;
}

void Renderer::CreateGraphBuffers() {
    glGenVertexArrays(1, &m_graphBuffer.vao);
    glGenBuffers(1, &m_graphBuffer.vbo);

    glBindVertexArray(m_graphBuffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_graphBuffer.vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::CreateGridBuffers() {
    glGenVertexArrays(1, &m_gridBuffer.vao);
    glGenBuffers(1, &m_gridBuffer.vbo);

    glBindVertexArray(m_gridBuffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer.vbo);

    // Create grid vertices
    std::vector<float> vertices;
    vertices.reserve(88 * 2); // 44 lines * 2 points per line

    // Grid lines
    for (int i = -5; i <= 5; ++i) {
        // Vertical lines
        vertices.push_back(static_cast<float>(i));
        vertices.push_back(-5.0f);
        vertices.push_back(static_cast<float>(i));
        vertices.push_back(5.0f);

        // Horizontal lines
        vertices.push_back(-5.0f);
        vertices.push_back(static_cast<float>(i));
        vertices.push_back(5.0f);
        vertices.push_back(static_cast<float>(i));
    }

    m_gridVertexCount = static_cast<int>(vertices.size() / 2);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

bool Renderer::SetupShaders() {
    const char* graphVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        uniform mat4 projection;
        uniform mat4 view;
        void main() {
            gl_Position = projection * view * vec4(aPos, 0.0, 1.0);
        }
    )";

    const char* graphFragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green color for graph
        }
    )";

    const char* gridVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
        }
    )";

    const char* gridFragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.5, 0.5, 0.5, 1.0); // Gray color for grid
        }
    )";

    // Compile and link graph shaders
    if (!CompileShader(graphVertexShaderSource, GL_VERTEX_SHADER, m_graphShader.vertexShader) ||
        !CompileShader(graphFragmentShaderSource, GL_FRAGMENT_SHADER, m_graphShader.fragmentShader) ||
        !LinkProgram(m_graphShader.vertexShader, m_graphShader.fragmentShader, m_graphShader.id)) {
        return false;
    }

    // Compile and link grid shaders
    if (!CompileShader(gridVertexShaderSource, GL_VERTEX_SHADER, m_gridShader.vertexShader) ||
        !CompileShader(gridFragmentShaderSource, GL_FRAGMENT_SHADER, m_gridShader.fragmentShader) ||
        !LinkProgram(m_gridShader.vertexShader, m_gridShader.fragmentShader, m_gridShader.id)) {
        return false;
    }

    return true;
}

void Renderer::UpdateMatrices() {
    int width, height;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &width, &height);

    m_projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);
    m_view = glm::mat4(1.0f);
}

} // namespace rendering
} // namespace plot_genius